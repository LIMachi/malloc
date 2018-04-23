/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_new_head.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 13:16:47 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/22 17:53:00 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

#include <stdint.h>

/*
** uint16_t
*/

#include <sys/mman.h>

/*
** void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
** int munmap(void *addr, size_t len)
*/

static inline void	*sif_attach_head(t_ma_header_bloc **head,
									t_ma_header_bloc *tmp)
{
	tmp->next = *head;
	if (*head != NULL)
	{
		tmp->prev = (*head)->prev;
		(*head)->prev = tmp;
	}
	else
		tmp->prev = NULL;
	return ((void*)(*head = tmp));
}

void				*ma_new_head_bloc(void **head, const t_ma_type_data td,
										size_t *index)
{
	t_ma_header_bloc	*tmp;
	uint16_t			*blocs;
	void				*data;
	size_t				i;
	size_t				j;

	write(1, "new_head_bloc\n", 14); //DEBUG
	if ((tmp = (t_ma_header_bloc*)mmap(0, td.header_size +
		td.pages_per_header * ma_handler()->page_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	tmp->prev = NULL;
	i = -1;
	blocs = (uint16_t*)&tmp->pages[td.pages_per_header];
	data = (void*)(((size_t)tmp) + td.header_size);
	while (++i < td.pages_per_header)
	{
		tmp->pages[i] = data + i * ma_handler()->page_size;
		j = -1;
		while (++j < td.blocs_per_page)
			blocs[i * td.blocs_per_page + j] = 0;
	}
	*index = 0;
	return (sif_attach_head((t_ma_header_bloc**)head, tmp));
}

void				*ma_new_head_list(void **head, const t_ma_type_data td,
										size_t *index)
{
	t_ma_header_link	*tmp;
	t_ma_header_link	*link;

	write(1, "new_head_list\n", 14); //DEBUG
	if ((tmp = (t_ma_header_link*)mmap(0, td.header_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	tmp->prev = NULL;
	tmp->size = td.header_size;
	link = (t_ma_header_link*)tmp->data;
	link->prev = NULL;
	link->next = NULL;
	link->size = ~USED & (td.header_size - sizeof(t_ma_header_link) -
		sizeof(t_ma_header_link));
	*index = 0;
	tmp->next = *head;
	if (*head != NULL)
	{
		tmp->prev = ((t_ma_header_link*)*head)->prev;
		((t_ma_header_link*)*head)->prev = tmp;
	}
	else
		tmp->prev = NULL;
	return ((void*)(*head = tmp));
}
