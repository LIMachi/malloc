/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_new_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 23:12:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/19 15:15:21 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

/*
** create a new pool of the chosen type, prepare it and link it
*/

t_ma_head	*ma_new_pool(size_t size, int type)
{
	t_ma_head	*out;
	t_ma_link	*link;
	size_t		size;

	if (type != MA_T_LARGE)
		size = g_ma_holder.td[type].pool_size;
	else
	{
		size = size + sizeof(t_ma_link) + sizeof(t_ma_head);
		size = (size / g_ma_holder.page_size + (size % g_ma_holder.page_size)) *
			g_ma_holder.page_size;
	}
	if ((out = (t_ma_head*)mmap(0, size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
		return (NULL);
	out->next = g_ma_holder.head[type];
	out->data->size = size - sizeof(t_ma_link) + sizeof(t_ma_head);
	out->data->allocated = 0;
	return ((g_ma_holder.head[type] = out));
}
