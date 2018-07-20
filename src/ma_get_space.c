/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_get_space.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 23:11:13 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/20 17:50:52 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <unistd.h>

static inline t_ma_link	*sif_cut(size_t nb, int type, t_ma_link *l,
								size_t total_size)
{
	t_ma_link	*tmp;

	if (type != MA_T_LARGE && nb + sizeof(t_ma_link) <= l->size && total_size
		+ sizeof(t_ma_link) + nb < g_ma_holder.td[type].pool_size)
	{
		tmp = ((t_ma_link*)&l->data[nb]);
		tmp->size = l->size - nb - sizeof(t_ma_link);
		tmp->allocated = 0;
		l->size = nb;
	}
	l->allocated = 1;
	return (l);
}

/*
** search a free block of at least nb size and cut it (type == 0 | 1) and flag
** it as used or return NULL if no valid place was found
*/

t_ma_link				*ma_get_space(size_t nb, int type)
{
	t_ma_head	*h;
	t_ma_link	*l;
	size_t		total_size;

	h = g_ma_holder.head[type];
	while (h != NULL)
	{
		l = h->data;
		total_size = sizeof(t_ma_head);
		while (1)
		{
			if (!l->allocated && l->size >= nb)
				return (sif_cut(nb, type, l, total_size));
			if ((total_size += sizeof(t_ma_link) + l->size) >=
					g_ma_holder.td[type].pool_size)
				break ;
			l = (t_ma_link*)&l->data[l->size];
		}
		h = h->next;
	}
	return (NULL);
}
