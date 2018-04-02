/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 15:08:13 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/02 17:40:47 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

/*
** struct, global, prototype
*/

#include <unistd.h>

/*
** int getpagesize(void)
*/

#if BONUS

# include <stdlib.h>

/*
** char *getenv(const char *name)
** int atexit(void (*func)(void))
*/

# include <inttypes.h> //FIXME: replace by libft include/functions

/*
** long long strtoll(const char *restrict str, char **restrict endptr, int base)
*/

# include <pthread.h>

/*
** pthread_mutex_t
** int pthread_mutex_lock(pthread_mutex_t *mutex)
** int pthread_mutex_unlock(pthread_mutex_t *mutex)
*/

#endif

struct s_ma_handler	g_ma_handler = {
	.flags = UNINITIALIZED,
	.tiny_size = 0,
	.tiny_zone = 0,
	.small_size = 0,
	.small_zone = 0,
	.page_size = 0,
	.guard_edges = 0,
	.scribble = 0,
	.tiny = 0,
	.small = 0,
	.large = 0
};

static inline void	sif_initial_alloc_tiny_small(void)
{
	(void)0;
}

#if BONUS

static void			sf_final_free(void)
{
	pthread_mutex_lock(&g_ma_mutex);
	(void)0;
	pthread_mutex_unlock(&g_ma_mutex);
}

static inline void	sif_load_env(void)
{
	char	*v;

	g_ma_handler.flags = (g_ma_handler.flags & ~UNINITIALIZED) | INITIALIZED;
	if (NULL != (v = getenv("MALLOC_SCRIBBLE")))
	{
		g_ma_handler.scribble = (char)strtoll(v, NULL, 0);
		g_ma_handler.flags |= SCRIBBLE;
	}
	if (NULL != (v = getenv("MALLOC_GUARD_EDGES")))
	{
		g_ma_handler.guard_edges = (size_t)strtoll(v, NULL, 0);
		g_ma_handler.flags |= GUARD_EDGES;
	}
	if (NULL != getenv("MALLOC_ALLOC_LOG"))
		g_ma_handler.flags |= ALLOC_LOG;
	if (NULL != getenv("MALLOC_FREE_LOG"))
		g_ma_handler.flags |= FREE_LOG;
	if (NULL != getenv("MALLOC_HEXDUMP"))
		g_ma_handler.flags |= HEXDUMP;
	if (NULL != getenv("MALLOC_FRAGMENTED"))
		g_ma_handler.flags |= FRAGMENTED;
	if (NULL != getenv("MALLOC_FINAL_FREE"))
		g_ma_handler.flags |= FINAL_FREE;
	if (NULL != getenv("MALLOC_NO_FREE"))
		g_ma_handler.flags |= NO_FREE;
}

inline void			malloc_init(void)
{
	size_t	t1;
	size_t	t2;

	if (g_ma_handler.flags & INITIALIZED)
		return ;
	sif_load_env();
	g_ma_handler.page_size = (size_t)getpagesize();
	t1 = HINT_TINY_SIZE;
	while (g_ma_handler.page_size / (t1 + g_ma_handler.guard_edges * 2) < 128)
		t1 >>= 1;
	g_ma_handler.tiny_size = t1;
	g_ma_handler.tiny_zone = g_ma_handler.page_size;
	t1 = g_ma_handler.page_size;
	t2 = HINT_SMALL_SIZE;
	while ((t2 + g_ma_handler.guard_edges * 2) > t1)
		t2 >>= 1;
	while (t1 / t2 < 128)
		t1 += g_ma_handler.page_size;
	g_ma_handler.small_size = t2;
	g_ma_handler.small_zone = t1;
	sif_initial_alloc_tiny_small();
	if (g_ma_handler.flags & FINAL_FREE)
		atexit(&sf_final_free);
}

#else

inline void			malloc_init(void)
{
	size_t	t1;
	size_t	t2;

	if (g_ma_handler.flags & INITIALIZED)
		return ;
	g_ma_handler.flags = INITIALIZED | SCRIBBLE;
	g_ma_handler.scribble = 0x42;
	g_ma_handler.page_size = (size_t)getpagesize();
	t1 = HINT_TINY_SIZE;
	while (g_ma_handler.page_size / (t1 + g_ma_handler.guard_edges * 2) < 128)
		t1 >>= 1;
	g_ma_handler.tiny_size = t1;
	g_ma_handler.tiny_zone = g_ma_handler.page_size;
	t1 = g_ma_handler.page_size;
	t2 = HINT_SMALL_SIZE;
	while ((t2 + g_ma_handler.guard_edges * 2) > t1)
		t2 >>= 1;
	while (t1 / t2 < 128)
		t1 += g_ma_handler.page_size;
	g_ma_handler.small_size = t2;
	g_ma_handler.small_zone = t1;
	sif_initial_alloc_tiny_small();
}

#endif
