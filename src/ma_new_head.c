/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_new_head.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 13:16:47 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/16 16:46:12 by hmartzol         ###   ########.fr       */
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

void	*ma_new_head_bloc(t_ma_type_data td)
{
	t_ma_header_small_tiny	*head;
	uint16_t				*blocs;
	void					*data;
	size_t					i;
	size_t					j;

	if ((head = (t_ma_header_small_tiny*)mmap(0, td.header_size +
		td.pages_per_header * g_ma_handler.page_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	head->next = NULL;
	head->prev = NULL;
	i = -1;
	blocs = (uint16_t*)&head->pages[td.pages_per_header];
	data = (void*)(((size_t)head) + td.header_size);
	while (++i < td.pages_per_header)
	{
		head->pages[i] = data + i * g_ma_handler.page_size;
		j = -1;
		while (++j < td.blocs_per_page)
			blocs[i * td.pages_per_header + j] = 0;
	}
	return (head);
}

void	*ma_new_head_list(t_ma_type_data td)
{
	t_ma_header_pool		*head;
	t_ma_header_pool_link	*link;

	if ((head = (t_ma_header_pool*)mmap(0, td.header_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	head->next = NULL;
	head->prev = NULL;
	head->total_size = td.header_size;
	link = (t_ma_header_pool_link*)head->data;
	link->prev = NULL;
	link->next = NULL;
	link->size = ~USED & (td.header_size - sizeof(t_ma_header_pool) -
		sizeof(t_ma_header_pool_link));
	return ((void*)head);
}
