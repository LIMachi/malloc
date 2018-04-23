/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aggressive_munmap.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 02:49:48 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/23 03:51:04 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"

#include <sys/mman.h>

/*
** int munmap(void *addr, size_t len)
*/

#include <stdint.h>

/*
** uint16_t
*/

/*
** forcefully munmap all unused pages, will never unmap the first header of tiny
** and small (but might unmap pages referenced by those headers)
*/

static inline void	sif_munmap_headers(t_ma_header_bloc *h,
										const t_ma_type_data td)
{
	size_t				page;
	int					used;
	t_ma_header_bloc	*tmp;

	while (h != NULL)
	{
		page = -1;
		used = 0;
		while (used == 0 && ++page < td.pages_per_header)
			if (h->pages[page] != NULL)
				used = 1;
		if (used == 0)
		{
			if (h->prev)
				h->prev->next = h->next;
			if (h->next)
				h->next->prev = h->prev;
			tmp = h->next;
			munmap(h, td.header_size);
			h = tmp;
		}
		else
			h = h->next;
	}
}

static inline void	sif_munmap_pages(t_ma_header_bloc *h,
									const t_ma_type_data td)
{
	size_t		page;
	size_t		bloc;
	uint16_t	*blocs;
	size_t		id;

	while (h != NULL)
	{
		page = -1;
		blocs = (uint16_t*)&h->pages[td.pages_per_header];
		while (++page < td.pages_per_header)
		{
			if (h->pages[page] == NULL)
				continue ;
			id = 0;
			bloc = -1;
			while (++bloc < td.blocs_per_page)
				id |= blocs[page * td.blocs_per_page + bloc];
			if (id == 0)
			{
				munmap(h->pages[page], ma_handler()->page_size);
				h->pages[page] = NULL;
			}
		}
		h = h->next;
	}
}

static inline void	sif_munmap_larges(void)
{
	t_ma_header_link	*head;
	t_ma_header_link	*l;
	t_ma_header_link	*tmp;

	head = ma_handler()->large;
	l = ma_handler()->large;
	while (l != NULL)
		if ((l->size & USED) == 0)
		{
			if (l == head)
				head = head->next;
			if (l->prev != NULL)
				l->prev->next = l->next;
			if (l->next != NULL)
				l->next->prev = l->prev;
			tmp = l->next;
			munmap(l, l->size & ~USED);
			l = tmp;
		}
		else
			l = l->next;
	ma_handler()->large = head;
}

static inline void	sif_munmap_link(t_ma_header_link *l)
{
	t_ma_header_link	*sub;
	t_ma_header_link	*tmp;
	int					used;

	while (l != NULL)
	{
		sub = (t_ma_header_link*)l->data;
		used = 0;
		while (sub != NULL || (used = (sub->size & USED)))
			sub = sub->next;
		if (used != USED)
		{
			if (l->prev != NULL)
				l->prev->next = l->next;
			if (l->next != NULL)
				l->next->prev = l->prev;
			tmp = l->next;
			munmap(l, l->size);
			l = tmp;
		}
		else
			l = l->next;
	}
}

void				aggressive_munmap(void)
{
	if (ma_handler()->flags & MODE_LIST)
	{
		sif_munmap_link(((t_ma_header_link*)ma_handler()->tiny)->next);
		sif_munmap_link(((t_ma_header_link*)ma_handler()->small)->next);
	}
	else
	{
		if (ma_handler()->tiny != NULL)
		{
			sif_munmap_pages(ma_handler()->tiny, ma_handler()->tiny_td);
			sif_munmap_headers(((t_ma_header_bloc*)ma_handler()->tiny)->next,
								ma_handler()->tiny_td);
		}
		if (ma_handler()->small != NULL)
		{
			sif_munmap_pages(ma_handler()->small, ma_handler()->small_td);
			sif_munmap_headers(((t_ma_header_bloc*)ma_handler()->small)->next,
								ma_handler()->small_td);
		}
	}
	sif_munmap_larges();
}
