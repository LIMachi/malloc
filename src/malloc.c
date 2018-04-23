/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 18:54:57 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/23 04:23:32 by hmartzol         ###   ########.fr       */
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

static inline int	sif_classify_size(size_t size)
{
	if (size <= ma_handler()->tiny_td.largest_size)
		return (TINY);
	if (size <= ma_handler()->small_td.largest_size)
		return (SMALL);
	return (LARGE);
}

static inline void	*sif_malloc_large(size_t size)
{
	(void)size;
	return (NULL);
}

#if BONUS

void				*ft_malloc(size_t size)
{
	int		type;
	size_t	index;
	void	*ptr;
	void	*h;

	pthread_mutex_lock(&g_ma_mutex);
	if (malloc_init(0) || size == 0)
		return (NULL);
	if ((type = sif_classify_size(size)) == TINY)
	{
		if ((ptr = ma_handler()->func.get_space(size, ma_handler()->tiny_td, ma_handler()->tiny)) == NULL)
			if ((h = ma_handler()->func.new_space(ma_handler()->tiny, ma_handler()->tiny_td, &index)) == NULL)
				return (NULL);
			else
				if ((ptr = ma_handler()->func.get_space(size, ma_handler()->tiny_td, &h)) == NULL)
					return (NULL);
				else
					return (ptr);
		else
			return (ptr);
	}
	if (type == SMALL)
	{
		if ((ptr = ma_handler()->func.get_space(size, ma_handler()->small_td, ma_handler()->small)) == NULL)
			if ((h = ma_handler()->func.new_space(ma_handler()->small, ma_handler()->small_td, &index)) == NULL)
				return (NULL);
			else
				if ((ptr = ma_handler()->func.get_space(size, ma_handler()->small_td, &h)) == NULL)
					return (NULL);
				else
					return (ptr);
		else
			return (ptr);
	}
	if (type == LARGE)
		return (sif_malloc_large(size));
	return (NULL);
	pthread_mutex_unlock(&g_ma_mutex);
	return (ptr);
}

#else

void				*ft_malloc(size_t size)
{
	int		type;
	size_t	index;
	void	*ptr;
	void	*h;

	if (malloc_init(0) || size == 0)
		return (NULL);
	if ((type = sif_classify_size(size)) == TINY)
	{
		if ((ptr = ma_handler()->func.get_space(size, ma_handler()->tiny_td, ma_handler()->tiny)) == NULL)
			if ((h = ma_handler()->func.new_space(ma_handler()->tiny, ma_handler()->tiny_td, &index)) == NULL)
				return (NULL);
			else
				if ((ptr = ma_handler()->func.get_space(size, ma_handler()->tiny_td, &h)) == NULL)
					return (NULL);
				else
					return (ptr);
		else
			return (ptr);
	}
	if (type == SMALL)
	{
		if ((ptr = ma_handler()->func.get_space(size, ma_handler()->small_td, ma_handler()->small)) == NULL)
			if ((h = ma_handler()->func.new_space(ma_handler()->small, ma_handler()->small_td, &index)) == NULL)
				return (NULL);
			else
				if ((ptr = ma_handler()->func.get_space(size, ma_handler()->small_td, &h)) == NULL)
					return (NULL);
				else
					return (ptr);
		else
			return (ptr);
	}
	if (type == LARGE)
		return (sif_malloc_large(size));
	return (NULL);
}

#endif

void				*malloc(size_t size)
{
	return (ft_malloc(size));
}
