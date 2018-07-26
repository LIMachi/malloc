/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   categorize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 23:01:01 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 18:50:55 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

inline int	ma_categorize(size_t size)
{
	if (size <= g_ma_holder.td[0].maximum_size)
		return (MA_T_TINY);
	if (size <= g_ma_holder.td[1].maximum_size)
		return (MA_T_SMALL);
	return (MA_T_LARGE);
}
