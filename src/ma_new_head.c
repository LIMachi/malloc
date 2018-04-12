/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_new_head.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 13:16:47 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/12 17:00:18 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

#include <sys/mman.h>

/*
** void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
** int munmap(void *addr, size_t len)
*/

#if BONUS

# include <errno.h>

/*
** extern int errno;
*/

t_ma_header_small_tiny	*ma_new_head(t_ma_type_data td)
{
	void					*ptr;
	void					**head;
	int						sav_err;
	size_t					i;

	if ((ptr = mmap(0, td.pages_per_header * g_ma_handler.page_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	if ((head = (void**)mmap(0, td.header_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
	{
		sav_err = errno;
		munmap(ptr, td.pages_per_header * g_ma_handler.page_size);
		errno = sav_err;
		return (NULL);
	}
	head[0] = NULL;
	head[1] = NULL;
	i = -1;
	while (++i < td.pages_per_header)
	{
		head[2 + i] = (void*)(((size_t)ptr) + i * g_ma_handler.page_size);
		((char*)(head + 2 + td.pages_per_header))[i] = 0;
	}
	return ((t_ma_header_small_tiny*)head);
}

#else

t_ma_header_small_tiny	*ma_new_head(t_ma_type_data td)
{
	void					*ptr;
	void					**head;
	size_t					i;

	if ((ptr = mmap(0, td.pages_per_header * g_ma_handler.page_size,
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	if ((head = (void**)mmap(0, td.header_size, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
	{
		munmap(ptr, td.pages_per_header * g_ma_handler.page_size);
		return (NULL);
	}
	head[0] = NULL;
	head[1] = NULL;
	i = -1;
	while (++i < td.pages_per_header)
	{
		head[2 + i] = (void*)(((size_t)ptr) + i * g_ma_handler.page_size);
		((char*)(head + 2 + td.pages_per_header))[i] = 0;
	}
	return ((t_ma_header_small_tiny*)head);
}

#endif
