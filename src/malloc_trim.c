/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_trim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 07:12:24 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/23 07:42:35 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <sys/mman.h>

static inline size_t	sif_malloc_trim(t_ma_head **h, int type)
{
	size_t		size;
	int			used;
	int			error;
	t_ma_link	*l;

	l = (*h)->data;
	used = 0;
	while (l != NULL && !(used != l->allocated))
		l = ma_next_block(*h, type, l, &error);
	size = 0;
	if (!used)
	{
		if ((*h)->prev != NULL)
			(*h)->prev->next = (*h)->next;
		if ((*h)->next)
			(*h)->next->prev = (*h)->prev;
		l = (t_ma_link*)(*h)->next;
		size = type != MA_T_LARGE ? g_ma_holder.td[type].pool_size :
			(*h)->data->size + sizeof(t_ma_head) + sizeof(t_ma_link);
		munmap(*h, size);
		*h = (t_ma_head*)l;
	}
	else
		*h = (*h)->next;
	return (size / g_ma_holder.page_size);
}

size_t					malloc_trim(void)
{
	int			type;
	size_t		count;
	t_ma_head	*h;

	type = -1;
	count = 0;
	while (++type)
	{
		if (type == MA_T_LARGE)
			h = g_ma_holder.head[MA_T_LARGE];
		else
			h = g_ma_holder.head[type] != NULL ?
				g_ma_holder.head[type]->next : NULL;
		while (h != NULL)
			count += sif_malloc_trim(&h, type);
	}
	return (count);
}
