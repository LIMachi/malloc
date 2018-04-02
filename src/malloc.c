/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 18:54:57 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/02 17:36:53 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

/*
** typedef size_t
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

# include <fcntl.h>

/*
** int open(const char *path, int oflag, ...)
*/

# include <pthread.h>

/*
** pthread_mutex_t
** int pthread_mutex_lock(pthread_mutex_t *mutex)
** int pthread_mutex_unlock(pthread_mutex_t *mutex)
*/

void	*smmap(size_t size, size_t page_size)
{
	void	*p;
	int		fd;

	if (size == 0)
		return (0);
	if (size % page_size)
		size = size - (size % page_size) + page_size;
	fd = open("/dev/zero", O_RDWR);
	p = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	close(fd);
	return (p);
}

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

void	*smmap(size_t size, size_t page_size)
{
	void	*p;

	if (size == 0)
		return (0);
	if (size % page_size)
		size = size - (size % page_size) + page_size;
	p = mmap(0, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	return (p);
}

void	*malloc(size_t size)
{
	(void)size;
	return (NULL);
}

#endif
