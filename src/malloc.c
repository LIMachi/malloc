/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:43:03 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 17:51:28 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <unistd.h>
#include <string.h>

MA_PRIVATE t_ma_link	*ma_malloc(size_t size, int type)
{
	t_ma_link	*l;

	if ((l = ma_get_space(size, type)) == NULL)
	{
		if (ma_new_pool(size, type) == NULL)
			return (NULL);
		l = ma_get_space(size, type);
	}
	if (l != NULL && g_ma_holder.bonus.flags & SCRIBBLE)
		memset(l->data, g_ma_holder.bonus.scribble, l->size);
	return (l);
}

MA_PUBLIC void			*malloc(size_t size)
{
	int			type;
	t_ma_link	*l;
	char		buff0[65];
	char		buff1[65];

	pthread_mutex_lock(&g_ma_mutex);
	if (!g_ma_holder.initialized)
		ma_init();
	++g_ma_holder.bonus.call_number;
	ma_debug_utoabuff(size, buff0, 10, "0123456789");
	ma_log("malloc", 3, "call to malloc with a size of ", buff0, " bytes");
	size += g_ma_holder.bonus.guard_edges * 2;
	type = ma_categorize(size);
	if ((l = ma_malloc(size, type)) == NULL)
		return ((void*)(pthread_mutex_unlock(&g_ma_mutex) * 0l));
	pthread_mutex_unlock(&g_ma_mutex);
	if (g_ma_holder.bonus.flags & ALLOC_LOG)
	{
		ma_debug_utoabuff((size_t)l->data + g_ma_holder.bonus.guard_edges,
			buff1, 16, "0123456789ABCDEF");
		ma_log("malloc", 4, "alloc of size: ", buff0,
								" stored at: 0x", buff1);
	}
	return (l->data + g_ma_holder.bonus.guard_edges);
}
