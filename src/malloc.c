/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 18:54:57 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/13 06:43:25 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

/*
** struct, global, prototype
*/

#include <sys/mman.h>

/*
** void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
** int munmap(void *addr, size_t len)
*/

#include <unistd.h>

/*
** int getpagesize(void)
** int close(int fd)
*/

#if BONUS

void	*malloc(size_t size)
{
	void	*ptr_out;

	pthread_mutex_lock(&g_ma_mutex);
	(void)size;
	ptr_out = NULL;
	pthread_mutex_unlock(&g_ma_mutex);
	return (ptr_out);
}

#else

void	*malloc(size_t size)
{
	size_t	nb_blocs;
	int		type;

	if (malloc_init() || size == 0)
		return (NULL);
	(void)type;
	(void)nb_blocs;
	return (NULL);
}

#endif
