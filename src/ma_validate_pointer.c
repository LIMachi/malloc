/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_validate_pointer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 14:57:43 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/23 07:04:09 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

MA_PRIVATE int		ma_validate_pointer(void *ptr, t_ma_found_link *f)
{
	int	err;

	f->type = -1;
	while (++f->type < 3)
	{
		f->head = g_ma_holder.head[f->type];
		while (f->head != NULL && !(f->before = NULL))
		{
			f->found = f->head->data;
			while (f->found != NULL)
			{
				if (f->found->data == ptr)
				{
					f->after = ma_next_block(f->head, f->type, f->found, &err);
					return (!err);
				}
				f->before = f->found;
				f->found = ma_next_block(f->head, f->type, f->found, &err);
				if (err)
					return (0);
			}
			f->head = f->head->next;
		}
	}
	return (f->type < 3);
}
