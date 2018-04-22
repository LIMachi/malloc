/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 15:08:13 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/22 17:42:29 by hmartzol         ###   ########.fr       */
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

# include <string.h>

/*
** int strcmp(const char *s1, const char *s2);
*/

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

pthread_mutex_t		g_ma_mutex;

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
	.large = NULL,
	.func = {
		.new_space = NULL,
		.get_space = NULL,
		.search_pointer = NULL
	}
};

static void			sf_calc_sizes(size_t page_size, double exp_multiplier,
							double next_exp_multiplier, t_ma_type_data *data)
{
	size_t	exponent;
	size_t	tmp;

	exponent = 0;
	tmp = page_size;
	while (tmp >>= 1)
		++exponent;
	data->bloc_size = 1 << (size_t)((double)exponent * exp_multiplier);
	data->largest_size = 1 << (size_t)((double)exponent * next_exp_multiplier);
	data->blocs_per_page = g_ma_handler.page_size / data->bloc_size;
	data->pages_per_header = (page_size - __SIZEOF_POINTER__ * 2) /
		(__SIZEOF_POINTER__ + data->blocs_per_page * 2);
	if (g_ma_handler.flags & MODE_LIST && (tmp = g_ma_handler.page_size))
	{
		while (tmp < sizeof(t_ma_header_link) +
				PREALLOC * (data->largest_size + sizeof(t_ma_header_link)))
			tmp += g_ma_handler.page_size;
		data->header_size = tmp;
		return ;
	}
	data->header_size = page_size;
	if ((g_ma_handler.page_size / data->largest_size)
		* data->pages_per_header < PREALLOC)
		sf_calc_sizes(page_size + g_ma_handler.page_size, exp_multiplier,
			next_exp_multiplier, data);
}

#if BONUS

static inline void	sif_load_env(struct s_ma_handler *h)
{
	char	*v;

	if (NULL != (v = getenv("MALLOC_SCRIBBLE")) && (h->flags |= SCRIBBLE))
		h->scribble = (char)strtoll(v, NULL, 0);
	if (NULL != (v = getenv("MALLOC_GUARD_EDGES")) && (h->flags |= GUARD_EDGES))
		h->guard_edges = (size_t)strtoll(v, NULL, 0);
	if (NULL != getenv("MALLOC_ALLOC_LOG"))
		h->flags |= ALLOC_LOG;
	if (NULL != getenv("MALLOC_FREE_LOG"))
		h->flags |= FREE_LOG;
	if (NULL != getenv("MALLOC_LAZY_ALIGN"))
		h->flags |= LAZY_ALIGN;
	if (NULL != getenv("MALLOC_HEXDUMP"))
		h->flags |= HEXDUMP;
	if (NULL != getenv("MALLOC_FRAGMENTED"))
		h->flags |= FRAGMENTED;
	if (NULL != getenv("MALLOC_NO_FREE"))
		h->flags |= NO_FREE;
	if (NULL != getenv("MALLOC_NO_UNMAP"))
		h->flags |= NO_UNMAP;
	if (NULL != (v = getenv("MALLOC_MODE")))
		h->flags |= MODE_LIST * (!strcmp(v, "LIST") ||
			(!strcmp(v, "DEFAULT") && MA_MODE_DEFAULT == MA_MODE_LIST));
	else
		h->flags |= MODE_LIST * (MA_MODE_DEFAULT == MA_MODE_LIST);
}

inline int			malloc_init(size_t index)
{
	if (g_ma_handler.flags & INITIALIZED)
		return (0);
	g_ma_handler.flags = (g_ma_handler.flags & ~UNINITIALIZED) | INITIALIZED;
	write(1, "loadding flags\n", 15); //DEBUG
	sif_load_env(&g_ma_handler);
	write(1, "flags ok\n", 9); //DEBUG
	g_ma_handler.page_size = (size_t)getpagesize();
	sf_calc_sizes(g_ma_handler.page_size, TINY_EXPONENT_MULTIPLIER,
		SMALL_EXPONENT_MULTIPLIER, &g_ma_handler.tiny_td);
	sf_calc_sizes(g_ma_handler.page_size, SMALL_EXPONENT_MULTIPLIER, 1.0,
		&g_ma_handler.small_td);
	write(1, "sizes ok\n", 9); //DEBUG
	if (g_ma_handler.flags & MODE_LIST)
		g_ma_handler.func = (t_ma_func){.new_space = &ma_new_head_list,
			.get_space = &ma_get_space_list,
			.search_pointer = &ma_search_pointer_list};
	else
		g_ma_handler.func = (t_ma_func){.new_space = g_ma_handler.flags &
			LAZY_ALIGN ? &ma_new_head_bloc : &ma_new_page,
			.get_space = &ma_get_space_bloc,
			.search_pointer = &ma_search_pointer_bloc};
	write(1, "preallocation\n", 14); //DEBUG
	if ((g_ma_handler.tiny = g_ma_handler.func.new_space(&g_ma_handler.tiny,
			g_ma_handler.tiny_td, &index)) == NULL)
		return (-1);
	if ((g_ma_handler.small = g_ma_handler.func.new_space(&g_ma_handler.small,
			g_ma_handler.small_td, &index)) == NULL)
		return (-1);
	write(1, "init finished\n", 14); //DEBUG
	return (0);
}

#else

inline int			malloc_init(size_t index)
{
	if (g_ma_handler.flags & INITIALIZED)
		return (0);
	g_ma_handler.flags = INITIALIZED |
		MODE_LIST * (MA_MODE_DEFAULT == MA_MODE_LIST);
	g_ma_handler.page_size = (size_t)getpagesize();
	sf_calc_sizes(g_ma_handler.page_size, TINY_EXPONENT_MULTIPLIER,
		SMALL_EXPONENT_MULTIPLIER, &g_ma_handler.tiny_td);
	sf_calc_sizes(g_ma_handler.page_size, SMALL_EXPONENT_MULTIPLIER, 1.0,
		&g_ma_handler.small_td);
	if (g_ma_handler.flags & MODE_LIST)
		g_ma_handler.func = (t_ma_func){.new_space = &ma_new_head_list,
			.get_space = &ma_get_space_list,
			.search_pointer = &ma_search_pointer_list};
	else
		g_ma_handler.func = (t_ma_func){.new_space = g_ma_handler.flags &
			LAZY_ALIGN ? &ma_new_head_bloc : &ma_new_page,
			.get_space = &ma_get_space_bloc,
			.search_pointer = &ma_search_pointer_bloc};
	if ((g_ma_handler.tiny = g_ma_handler.func.new_space(&g_ma_handler.tiny,
			g_ma_handler.tiny_td, &index)) == NULL)
		return (-1);
	if ((g_ma_handler.small = g_ma_handler.func.new_space(&g_ma_handler.small,
			g_ma_handler.small_td, &index)) == NULL)
		return (-1);
	return (0);
}

#endif
