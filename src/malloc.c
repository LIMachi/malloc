/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:43:03 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/23 09:13:38 by hmartzol         ###   ########.fr       */
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

	if (!g_ma_holder.initialized)
		ma_init();
	size += g_ma_holder.bonus.guard_edges * 2;
	type = ma_categorize(size);
	if ((l = ma_malloc(size, type)) == NULL)
		return (NULL);
	return (l->data + g_ma_holder.bonus.guard_edges);
}
