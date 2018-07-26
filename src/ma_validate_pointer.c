/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_validate_pointer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 14:57:43 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 18:54:04 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

static inline void	error(void *ptr)
{
	char	buff[65];

	ma_debug_utoabuff((size_t)ptr, buff, 16, "0123456789ABCDEF");
	ma_error("ma_validate_pointer", 3, "invalid pointer 0x", buff,
		" cannot be resolved in any pool");
}

int					ma_validate_pointer(void *ptr, t_ma_found_link *f)
{
	int	err;

	f->type = -1;
	while (++f->type < 3 && ((f->head = g_ma_holder.head[f->type]) || 1))
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
	if (f->type >= 3)
		error(ptr);
	return (f->type < 3);
}
