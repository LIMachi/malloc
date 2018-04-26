/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 19:10:55 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/26 03:55:35 by hmartzol         ###   ########.fr       */
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

# include <stdlib.h>

/*
** void exit(int code)
*/

#endif

static inline int	sif_address_error(void *ptr, const char *error, int mode)
{
	(void)ptr;
	(void)error;
	(void)mode;
	return (0);
}

static inline void	sif_log(void *ptr, void *tmp, int type, size_t index)
{
	(void)ptr;
	(void)tmp;
	(void)type;
	(void)index;
}

static inline void	sif_free(void *ptr, void *tmp, int type, size_t index)
{
	(void)ptr;
	(void)tmp;
	(void)type;
	(void)index;
}

#if BONUS

void				free(void *ptr)
{
	void				*tmp;
	int					type;
	size_t				index;
	static const char	*errors[4] = {

	"Invalid Pointer: Pointer %p passed to free %s"
	"wasn't allocated first\n",
	"is already free\n",
	"wasn't aligned with a valid address but was part of an allocation\n"};
	pthread_mutex_lock(&g_ma_mutex);
	if ((tmp = ma_handler()->func.search_pointer((const size_t)ptr, &type,
		&index)) == NULL)
	{
		if (ma_handler()->flags & INVALID_POINTER_ERROR)
			exit(sif_address_error(ptr, errors[-index], 1));
		if (ma_handler()->flags & FREE_LOG)
			sif_address_error(ptr, errors[-index], 0);
		return ;
	}
	if (ma_handler()->flags & FREE_LOG)
		sif_log(ptr, tmp, type, index);
	if (ma_handler()->flags & NO_FREE)
		return ;
	sif_free(ptr, tmp, type, index);
	pthread_mutex_unlock(&g_ma_mutex);
}

#else

void				free(void *ptr)
{
	void				*tmp;
	int					type;
	size_t				index;

	if ((tmp = ma_handler()->func.search_pointer((const size_t)ptr, &type,
		&index)) == NULL)
		return ;
	sif_free(ptr, tmp, type, index);
}

#endif
