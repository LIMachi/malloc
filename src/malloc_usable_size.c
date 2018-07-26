/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_usable_size.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 07:12:45 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 13:56:25 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

size_t	malloc_usable_size(void *ptr)
{
	t_ma_found_link	f;

	if (ptr == NULL)
		return (0);
	pthread_mutex_lock(&g_ma_mutex);
	if (ma_validate_pointer(ptr - g_ma_holder.bonus.guard_edges, &f)
			&& f.found->allocated)
	{
		pthread_mutex_unlock(&g_ma_mutex);
		return (f.found->size - g_ma_holder.bonus.guard_edges * 2);
	}
	pthread_mutex_unlock(&g_ma_mutex);
	return (0);
}
