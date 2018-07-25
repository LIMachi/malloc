/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:43:03 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/25 20:47:54 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <unistd.h>

MA_PRIVATE t_ma_link	*ma_malloc(size_t size, int type)
{
	t_ma_link	*l;

	if ((l = ma_get_space(size, type)) == NULL)
	{
		if (ma_new_pool(size, type) == NULL)
			return (NULL);
		l = ma_get_space(size, type);
	}
	return (l);
}

MA_PUBLIC void			*malloc(size_t size)
{
	int			type;
	t_ma_link	*l;
	char		buff0[65];
	char		buff1[65];

	if (!g_ma_holder.initialized)
		ma_init();
	++g_ma_holder.bonus.call_number;
	size += g_ma_holder.bonus.guard_edges * 2;
	type = ma_categorize(size);
	if ((l = ma_malloc(size, type)) == NULL)
		return (NULL);
	if (g_ma_holder.bonus.flags & ALLOC_LOG)
	{
		ma_debug_utoabuff(size - g_ma_holder.bonus.guard_edges * 2, buff0, 10, "0123456789");
		ma_debug_utoabuff((size_t)l->data + g_ma_holder.bonus.guard_edges,
			buff1, 16, "0123456789ABCDEF");
		ma_log("malloc", 4, "alloc of size: ", buff0,
								" stored at: 0x", buff1);
	}
	return (l->data + g_ma_holder.bonus.guard_edges);
}
