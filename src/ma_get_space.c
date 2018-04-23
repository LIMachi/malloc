/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_get_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 15:02:37 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/23 04:18:40 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

#include <stdint.h>

/*
** uint16_t
*/

static inline uint16_t	sif_get_new_id(t_ma_header_bloc *head, t_ma_type_data td)
{
	size_t		page;
	size_t		bloc;
	uint8_t		tmp[1 << 16];
	uint16_t	id;
	uint16_t	*blocs;

	id = 0;
	blocs = (uint16_t*)&head->pages[td.pages_per_header];
	tmp[0] = 0;
	while (++id != 0)
		tmp[id] = 0;
	page = -1;
	while (++page < td.pages_per_header)
		if (head->pages[page] != NULL && (bloc = -1))
			while (++bloc < td.blocs_per_page)
				tmp[blocs[page * td.blocs_per_page + bloc]] = 1;
	id = 0;
	while (++id != 0)
		if (tmp[id] == 0)
			return (id);
	return (0);
}

static inline void		*sif_get_space(size_t size, t_ma_type_data td,
										t_ma_header_bloc *head, size_t st[2])
{
	uint16_t	tmp;
	size_t		contiguous_blocs;
	uint16_t	*blocs;
	size_t		nb_blocs;

	nb_blocs = 0;
	while (nb_blocs * td.bloc_size < size)
		++nb_blocs;
	contiguous_blocs = 0;
	blocs = (uint16_t*)&head->pages[td.pages_per_header];
	if (st[1] < td.blocs_per_page)
		while (contiguous_blocs < nb_blocs &&
				contiguous_blocs + st[1] < td.blocs_per_page &&
				!(blocs[st[0] * td.blocs_per_page + st[1] + contiguous_blocs]))
			++contiguous_blocs;
	if (contiguous_blocs == nb_blocs && (tmp = sif_get_new_id(head, td)))
	{
		while (contiguous_blocs--)
			blocs[st[0] * td.blocs_per_page + st[1] + contiguous_blocs] = tmp;
		return ((void*)(((char*)head->pages[st[0]]) + st[1] * td.bloc_size));
	}
	return (NULL);
}

/*
** will search a space large enough to contain size and reserv it
** will return NULL if no space can be reserved (signaling a need to allocate
** new pages)
*/

void					*ma_get_space_bloc(size_t size, t_ma_type_data td,
										void **head)
{
	size_t				page;
	size_t				first_bloc;
	void				*tmp;
	t_ma_header_bloc	*h;
	uint16_t			*blocs;

	write(1, "get_space_bloc\n", 15); //DEBUG
	h = (t_ma_header_bloc*)*head;
	while (h != NULL && (page = -1))
	{
		while (++page < td.pages_per_header && (first_bloc = -1))
		{
			if (h->pages[page] == NULL)
				continue ;
			first_bloc = 0;
			blocs = (uint16_t*)&h->pages[td.pages_per_header];
			while (first_bloc < td.blocs_per_page && blocs[page * td.blocs_per_page + first_bloc])
				++first_bloc;
			if ((tmp = sif_get_space(size, td, h,
					(size_t[2]){page, first_bloc})) != NULL)
				return (tmp);
		}
		h = h->next;
	}
	return (NULL);
}

void					*ma_get_space_list(size_t size, t_ma_type_data td,
										void **head)
{
	t_ma_header_link	*link;
	t_ma_header_link	*tmp;

	write(1, "get_space_list\n", 15); //DEBUG
	(void)td;
	while (*head != NULL)
	{
		link = (t_ma_header_link*)(((t_ma_header_link*)*head)->data);
		while (link != NULL &&
				((link->size & USED) || (link->size & ~USED) < size))
			link = link->next;
		if (link == NULL && _(*head = ((t_ma_header_link*)*head)->next, 1))
			continue ;
		if ((link->size & ~USED) >= size + sizeof(t_ma_header_link))
		{
			tmp = link->next;
			link->next = (t_ma_header_link*)&link->data[size];
			*link->next = (t_ma_header_link){.prev = link, .next = tmp,
	.size = (link->prev->size & ~USED) - size - sizeof(t_ma_header_link)};
			link->size = size & USED;
		}
		else
			link->size |= USED;
		return ((void*)link->data);
	}
	return (NULL);
}
