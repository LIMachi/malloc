/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 22:30:41 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/21 20:21:03 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

#include <unistd.h>

/*
** int getpagesize(void)
*/

t_ma_holder						g_ma_holder = {
	.initialized = 0,
	.page_size = 0,
	.pool_size = MA_DEFAULT_POOL_SIZE,
	.td = {
		{
			.minimum_size = 0,
			.maximum_size = 0,
			.pool_size = 0,
		},
		{
			.minimum_size = 0,
			.maximum_size = 0,
			.pool_size = 0,
		},
		{
			.minimum_size = 0,
			.maximum_size = 0,
			.pool_size = 0,
		}
	},
	.head = {NULL, NULL, NULL}
# ifdef BONUS
	, .bonus = {
		.flags = 0,
		.guard_edges = 0,
		.scribble = '\0',
		.pages_mapped = 0,
		.pages_writen = 0,
		.log_fd = -1
	}
# endif
};

#ifdef BONUS

# include <string.h> //FIXME: replace by libft functions

/*
** int strcmp(const char *s1, const char *s2);
*/

# include <stdlib.h>

/*
** char *getenv(const char *name)
** int atexit(void (*func)(void))
*/

# include <inttypes.h> //FIXME: replace by libft functions

/*
** long long strtoll(const char *restrict str, char **restrict endptr, int base)
*/

static inline void	sif_bonus1(t_ma_bonus *b)
{
	if (NULL != getenv("MALLOC_EXIT_ON_ERROR"))
		b->flags |= EXIT_ON_ERROR;
	if (NULL != getenv("MALLOC_NO_FREE"))
		b->flags |= NO_FREE;
	if (NULL != getenv("MALLOC_FRAGMENTED"))
		b->flags |= FRAGMENTED;
}

static inline void	sif_bonus0(t_ma_bonus *b)
{
	char	*v;

	if (NULL != (v = getenv("MALLOC_LOG_FILE")) &&
			(b->flags |= FILE_LOG))
		if (-1 == (b->log_fd = open(v, O_WRONLY | O_CREAT | O_APPEND, 0500)))
			return ((size_t)-2);
	if (NULL != (v = getenv("MALLOC_SCRIBBLE")) && (b->flags |= SCRIBBLE))
		b->scribble = (char)strtoll(v, NULL, 0);
	if (NULL != (v = getenv("MALLOC_GUARD_EDGES")) && (b->flags |= GUARD_EDGES))
		b->guard_edges = (size_t)strtoll(v, NULL, 0);
	if (NULL != getenv("MALLOC_ALLOC_LOG"))
		b->flags |= ALLOC_LOG;
	if (NULL != getenv("MALLOC_FREE_LOG"))
		b->flags |= FREE_LOG;
	if (NULL != getenv("MALLOC_LAZY_ALIGN"))
		b->flags |= LAZY_ALIGN;
	if (NULL != getenv("MALLOC_HEXDUMP"))
		b->flags |= HEXDUMP;
	if (NULL != (v = getenv("MALLOC_MODE")))
		b->flags |= MODE_LIST * (!strcmp(v, "LIST") ||
			(!strcmp(v, "DEFAULT") && MA_MODE_DEFAULT == MA_MODE_LIST));
	else
		b->flags |= MODE_LIST * (MA_MODE_DEFAULT == MA_MODE_LIST);
	if (NULL != getenv("MALLOC_NO_UNMAP"))
		b->flags |= NO_UNMAP;
}

MA_PRIVATE void		ma_init(void)
{
	size_t	page_size_power;
	size_t	tmp;

	g_ma_holder.page_size = (size_t)getpagesize();
	tmp = g_ma_holder.page_size;
	page_size_power = 0;
	while (tmp >>= 1)
		++page_size_power;
	g_ma_holder.td[0].maximum_size = 1 << (page_size_power / 3);
	g_ma_holder.td[1].minimum_size = g_ma_holder.td[0].maximum_size;
	g_ma_holder.td[1].maximum_size = 1 << 2 * (page_size_power / 3);
	g_ma_holder.td[2].minimum_size = g_ma_holder.td[1].maximum_size;
	g_ma_holder.td[2].maximum_size = (size_t)-1;
	tmp = sizeof(t_ma_head) + g_ma_holder.pool_size *
		(g_ma_holder.td[0].maximum_size + sizeof(t_ma_link));
	g_ma_holder.td[0].pool_size = (tmp / g_ma_holder.page_size +
		!!(tmp % g_ma_holder.page_size)) * g_ma_holder.page_size;
	tmp = sizeof(t_ma_head) + g_ma_holder.pool_size *
		(g_ma_holder.td[1].maximum_size + sizeof(t_ma_link));
	g_ma_holder.td[1].pool_size = (tmp / g_ma_holder.page_size +
		!!(tmp % g_ma_holder.page_size)) * g_ma_holder.page_size;
	sif_bonus0(&g_ma_holder.bonus);
	sif_bonus1(&g_ma_holder.bonus);
	g_ma_holder.initialized = 1;
}

#else

MA_PRIVATE void 	ma_init(void)
{
	size_t	page_size_power;
	size_t	tmp;

	g_ma_holder.page_size = (size_t)getpagesize();
	tmp = g_ma_holder.page_size;
	page_size_power = 0;
	while (tmp >>= 1)
		++page_size_power;
	// char buff[42];
	// write(1, "page size power: ", 17);
	// write(1, buff, ma_debug_itoabuff(page_size_power, buff));
	// write(1, "\n", 1);
	g_ma_holder.td[0].maximum_size = 1 << (page_size_power / 2);
	g_ma_holder.td[1].minimum_size = g_ma_holder.td[0].maximum_size;
	g_ma_holder.td[1].maximum_size = 1 << (page_size_power);
	g_ma_holder.td[2].minimum_size = g_ma_holder.td[1].maximum_size;
	g_ma_holder.td[2].maximum_size = (size_t)-1;
	tmp = sizeof(t_ma_head) + g_ma_holder.pool_size *
		(g_ma_holder.td[0].maximum_size + sizeof(t_ma_link));
	g_ma_holder.td[0].pool_size = (tmp / g_ma_holder.page_size +
		!!(tmp % g_ma_holder.page_size)) * g_ma_holder.page_size;
	tmp = sizeof(t_ma_head) + g_ma_holder.pool_size *
		(g_ma_holder.td[1].maximum_size + sizeof(t_ma_link));
	g_ma_holder.td[1].pool_size = (tmp / g_ma_holder.page_size +
		!!(tmp % g_ma_holder.page_size)) * g_ma_holder.page_size;
	g_ma_holder.initialized = 1;
	// write(1, "sizes: \npool tiny: ", 19);
	// write(1, buff, ma_debug_itoabuff(g_ma_holder.td[0].pool_size, buff));
	// write(1, "\npool small: ", 13);
	// write(1, buff, ma_debug_itoabuff(g_ma_holder.td[1].pool_size, buff));
	// write(1, "\nmax size tiny: ", 16);
	// write(1, buff, ma_debug_itoabuff(g_ma_holder.td[0].maximum_size, buff));
	// write(1, "\nmax size small: ", 17);
	// write(1, buff, ma_debug_itoabuff(g_ma_holder.td[1].maximum_size, buff));
	// write(1, "\n", 1);
}

#endif
