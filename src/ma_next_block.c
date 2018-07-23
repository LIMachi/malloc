/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_next_block.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 05:56:52 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/23 06:57:02 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

MA_PRIVATE t_ma_link	*ma_next_block(t_ma_head *h, int type, t_ma_link *l,
										int *error)
{
	size_t	next_abs;

	*error = 0;
	if (type == MA_T_LARGE)
		return (NULL);
	next_abs = (size_t)l->data - (size_t)h + l->size;
	if (next_abs > g_ma_holder.td[type].pool_size)
		*error = -1;
	if (next_abs >= g_ma_holder.td[type].pool_size)
		return (NULL);
	return ((t_ma_link*)&l->data[l->size]);
}
