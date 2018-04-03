/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 19:10:55 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/03 13:06:55 by hmartzol         ###   ########.fr       */
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
*/

#if BONUS

void	free(void *ptr)
{
	pthread_mutex_lock(&g_ma_mutex);
	(void)ptr;
	pthread_mutex_unlock(&g_ma_mutex);
}

#else

void	free(void *ptr)
{
	(void)ptr;
}

#endif
