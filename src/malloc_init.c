/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 15:08:13 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/10 02:56:32 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

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

#endif

struct s_ma_handler	g_ma_handler = {
	.flags = UNINITIALIZED,
	.tiny_bloc = 0,
	.tiny_page = 0,
	.tiny_head_pages = 0,
	.small_bloc = 0,
	.small_page = 0,
	.small_head_pages = 0,
	.page_size = 0,
	.guard_edges = 0,
	.scribble = 0,
	.tiny = NULL,
	.small = NULL,
	.large = NULL
};

static inline void	sif_initial_alloc_tiny_small(void)
{
	(void)0;
}

#if BONUS

pthread_mutex_t		g_ma_mutex;

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
	t1 = g_ma_handler.page_size;
	t2 = 1;
	while (t1 >>= 1)
		++t2;
	t1 = t2 / 3;
	g_ma_handler.tiny_bloc = 1 << t1;
	g_ma_handler.small_bloc = 1 << (t2 - t1);
	g_ma_handler.tiny_page = g_ma_handler.page_size / g_ma_handler.tiny_bloc;
	g_ma_handler.small_page = g_ma_handler.page_size / g_ma_handler.small_bloc;
	g_ma_handler.tiny_head_pages = (g_ma_handler.page_size
		- __SIZEOF_POINTER__ * 2) / (__SIZEOF_POINTER__
		+ g_ma_handler.tiny_page * 2);
	g_ma_handler.small_head_pages = (g_ma_handler.page_size
		- __SIZEOF_POINTER__ * 2) / (__SIZEOF_POINTER__
		+ g_ma_handler.small_page * 2);
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
	g_ma_handler.flags = INITIALIZED;
	g_ma_handler.page_size = (size_t)getpagesize();
	t1 = g_ma_handler.page_size;
	t2 = 1;
	while (t1 >>= 1)
		++t2;
	t1 = t2 / 3;
	g_ma_handler.tiny_bloc = 1 << t1;
	g_ma_handler.small_bloc = 1 << (t2 - t1);
	g_ma_handler.tiny_page = g_ma_handler.page_size / g_ma_handler.tiny_bloc;
	g_ma_handler.small_page = g_ma_handler.page_size / g_ma_handler.small_bloc;
	g_ma_handler.tiny_head_pages = (g_ma_handler.page_size
		- __SIZEOF_POINTER__ * 2) / (__SIZEOF_POINTER__
		+ g_ma_handler.tiny_page * 2);
	g_ma_handler.small_head_pages = (g_ma_handler.page_size
		- __SIZEOF_POINTER__ * 2) / (__SIZEOF_POINTER__
		+ g_ma_handler.small_page * 2);
	sif_initial_alloc_tiny_small();
}

#endif
