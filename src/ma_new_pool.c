/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_new_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 23:12:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 18:53:31 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <sys/mman.h>

/*
** create a new pool of the chosen type, prepare it and link it
*/

t_ma_head	*ma_new_pool(size_t size,
		int type)
{
	t_ma_head	*out;

	if (type != MA_T_LARGE)
		size = g_ma_holder.td[type].pool_size;
	else
	{
		size = size + sizeof(t_ma_link) + sizeof(t_ma_head);
		size = (size / g_ma_holder.page_size + !!(size % g_ma_holder.page_size))
			* g_ma_holder.page_size;
	}
	if ((out = (t_ma_head*)mmap(0, size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANON, -1, 0)) == MAP_FAILED)
		return ((t_ma_head*)ma_error("ma_new_pool", 1,
			"out of memory (mmap failure)"));
		out->next = g_ma_holder.head[type];
	out->prev = NULL;
	if (out->next != NULL)
		out->next->prev = out;
	out->data->size = size - sizeof(t_ma_link) - sizeof(t_ma_head);
	out->data->allocated = 0;
	return ((g_ma_holder.head[type] = out));
}
