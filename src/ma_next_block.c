/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_next_block.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 05:56:52 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 18:51:43 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

t_ma_link	*ma_next_block(t_ma_head *h, int type, t_ma_link *l,
						int *error)
{
	size_t	next_abs;
	char	buff0[65];
	char	buff1[65];

	*error = 0;
	if (type == MA_T_LARGE)
		return (NULL);
	next_abs = (size_t)l->data - (size_t)h + l->size;
	if (next_abs > g_ma_holder.td[type].pool_size)
	{
		ma_debug_utoabuff((size_t)l->data, buff0, 16, "0123456789ABCDEF");
		ma_debug_utoabuff((size_t)h, buff1, 16, "0123456789ABCDEF");
		ma_error("ma_next_block", 4, "corrupt block found near 0x", buff0,
									" in pool 0x", buff1);
		*error = -1;
	}
	if (next_abs >= g_ma_holder.td[type].pool_size)
		return (NULL);
	return ((t_ma_link*)&l->data[l->size]);
}
