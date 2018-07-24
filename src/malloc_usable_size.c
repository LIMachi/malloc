/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_usable_size.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 07:12:45 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/23 08:52:56 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

size_t	malloc_usable_size(void *ptr)
{
	t_ma_found_link	f;

	if (ptr == NULL)
		return (0);
	if (ma_validate_pointer(ptr - g_ma_holder.bonus.guard_edges, &f)
			&& f.found->allocated)
		return (f.found->size - g_ma_holder.bonus.guard_edges * 2);
	return (0);
}
