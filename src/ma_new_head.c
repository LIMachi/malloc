/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_new_head.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 13:16:47 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/12 23:24:51 by hmartzol         ###   ########.fr       */
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

t_ma_header_small_tiny	*ma_new_head(t_ma_type_data td)
{
	void					**head;
	size_t					i;
	size_t					j;

	if ((head = (void**)mmap(0, td.header_size + td.pages_per_header *
		g_ma_handler.page_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	head[0] = NULL;
	head[1] = NULL;
	i = -1;
	while (++i < td.pages_per_header)
	{
		head[2 + i] = (void*)(((size_t)head) + i * g_ma_handler.page_size +
			td.header_size);
		j = -1;
		while (++j < td.blocs_per_page)
			((uint16_t*)(head + (2 + td.pages_per_header) *
				__SIZEOF_POINTER__))[i * td.pages_per_header + j] = 0;
	}
	return ((t_ma_header_small_tiny*)head);
}
