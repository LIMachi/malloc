/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_validate_pointer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 14:57:43 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/21 20:21:44 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

static inline int	sif_validate_pointer(void *ptr, int type,
										t_ma_found_link *f)
{
	size_t	total_size;

	f->head = g_ma_holder.head[type];
	while (f->head != NULL && (total_size = sizeof(t_ma_head)))
	{
		f->before = NULL;
		f->found = f->head->data;
		while (1)
		{
			if (f->found->data == ptr)
			{
				f->after = ((total_size + sizeof(t_ma_link) + f->found->size)
					>= g_ma_holder.td[type].pool_size) ? NULL :
					(t_ma_link*)&f->found->data[f->found->size];
				return (0);
			}
			if ((total_size += sizeof(t_ma_link) + f->found->size)
					>= g_ma_holder.td[type].pool_size)
				break ;
			f->before = f->found;
			f->found = (t_ma_link*)&f->found->data[f->found->size];
		}
		f->head = f->head->next;
	}
	return (1);
}

MA_PRIVATE int		ma_validate_pointer(void *ptr, t_ma_found_link *f)
{
	f->type = 0;
	while (f->type < 3 && sif_validate_pointer(ptr, f->type, f))
		++f->type;
	return (f->type < 3);
}
