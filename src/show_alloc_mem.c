/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 20:24:37 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/22 15:59:52 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <unistd.h>

MA_PUBLIC void	show_alloc_mem(void)
{
	t_ma_head			*h;
	// t_ma_link			*l;
	char				buff[65];
	static const char	base[17] = "0123456789ABCDEF";

	h = g_ma_holder.head[MA_T_TINY];
	while (h != NULL)
	{
		write(1, "TINY : 0x", 9);
		write(1, buff, ma_debug_utoabuff((size_t)h, buff, 16, base));
		write(1, "\n", 1);
		h = h->next;
	}
	h = g_ma_holder.head[MA_T_SMALL];
	while (h != NULL)
	{
		write(1, "SMALL : 0x", 10);
		write(1, buff, ma_debug_utoabuff((size_t)h, buff, 16, base));
		write(1, "\n", 1);
		h = h->next;
	}
	h = g_ma_holder.head[MA_T_LARGE];
	while (h != NULL)
	{
		write(1, "LARGE : 0x", 10);
		write(1, buff, ma_debug_utoabuff((size_t)h, buff, 16, base));
		write(1, "\n0x", 3);
		write(1, buff, ma_debug_utoabuff((size_t)h->data->data, buff, 16, base));
		write(1, " - 0x", 5);
		write(1, buff, ma_debug_utoabuff((size_t)h->data->data + h->data->size, buff, 16, base));
		write(1, " : ", 3);
		write(1, buff, ma_debug_utoabuff(h->data->size, buff, 10, base));
		write(1, " bytes\n", 7);
		h = h->next;
	}
}
