/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 15:08:13 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/12 17:06:01 by hmartzol         ###   ########.fr       */
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
	.tiny_td = {
		.bloc_size = 0,
		.blocs_per_page = 0,
		.pages_per_header = 0,
		.header_size = 0,
		.largest_size = 0
	},
	.small_td = {
		.bloc_size = 0,
		.blocs_per_page = 0,
		.pages_per_header = 0,
		.header_size = 0,
		.largest_size = 0
	},
	.page_size = 0,
	.guard_edges = 0,
	.scribble = 0,
	.tiny = NULL,
	.small = NULL,
	.large = NULL
};

static void			sf_calc_sizes(size_t page_size, double exp_multiplier,
							double next_exp_multiplier, t_ma_type_data *data)
{
	size_t	exponent;
	size_t	tmp;

	exponent = 1;
	tmp = page_size;
	while (tmp >>= 1)
		++exponent;
	data->bloc_size = 1 << (size_t)((double)exponent * exp_multiplier);
	data->largest_size = 1 << (size_t)((double)exponent * next_exp_multiplier);
	data->blocs_per_page = g_ma_handler.page_size / data->bloc_size;
	data->pages_per_header = (page_size - __SIZEOF_POINTER__ * 2) /
		(__SIZEOF_POINTER__ + data->blocs_per_page);
	data->header_size = page_size;
	if ((g_ma_handler.page_size / data->largest_size)
		* data->pages_per_header < PREALLOC)
		sf_calc_sizes(page_size + g_ma_handler.page_size, exp_multiplier,
			next_exp_multiplier, data);
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

inline int			malloc_init(void)
{
	if (g_ma_handler.flags & INITIALIZED)
		return (0);
	sif_load_env();
	g_ma_handler.page_size = (size_t)getpagesize();
	sf_calc_sizes(g_ma_handler.page_size, TINY_EXPONENT_MULTIPLIER,
		SMALL_EXPONENT_MULTIPLIER, &g_ma_handler.tiny_td);
	sf_calc_sizes(g_ma_handler.page_size, SMALL_EXPONENT_MULTIPLIER, 1.0,
		&g_ma_handler.small_td);
	if ((g_ma_handler.tiny = ma_new_head(g_ma_handler.tiny_td)) == NULL)
		return (-1);
	if ((g_ma_handler.small = ma_new_head(g_ma_handler.small_td)) == NULL)
		return (-1);
	if (g_ma_handler.flags & FINAL_FREE)
		atexit(&sf_final_free);
	return (0);
}

#else

inline int			malloc_init(void)
{
	if (g_ma_handler.flags & INITIALIZED)
		return (0);
	g_ma_handler.flags = INITIALIZED;
	g_ma_handler.page_size = (size_t)getpagesize();
	sf_calc_sizes(g_ma_handler.page_size, TINY_EXPONENT_MULTIPLIER,
		SMALL_EXPONENT_MULTIPLIER, &g_ma_handler.tiny_td);
	sf_calc_sizes(g_ma_handler.page_size, SMALL_EXPONENT_MULTIPLIER, 1.0,
		&g_ma_handler.small_td);
	if ((g_ma_handler.tiny = ma_new_head(g_ma_handler.tiny_td)) == NULL)
		return (-1);
	if ((g_ma_handler.small = ma_new_head(g_ma_handler.small_td)) == NULL)
		return (-1);
	return (0);
}

#endif
