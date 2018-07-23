/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 20:24:37 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/23 06:42:06 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <unistd.h>

static inline void	print_array(void *data, size_t size)
{
	char				buff[65];
	static const char	base[16] = "0123456789ABCDEF";
	size_t				len;

	len = ma_debug_utoabuff((size_t)data, buff, 16, base);
	write(1, "0x0000000000", 12 - len);
	write(1, buff, len);
	len = ma_debug_utoabuff((size_t)data + size, buff, 16, base);
	write(1, " - 0x0000000000", 15 - len);
	write(1, buff, len);
	write(1, " : ", 3);
	write(1, buff, ma_debug_utoabuff(size, buff, 10, base));
	write(1, " bytes\n", 7);
}

MA_PUBLIC void	show_alloc_mem(void)
{
	t_ma_head			*h;
	t_ma_link			*l;
	int					type;
	int					error;
	char				buff[65];

	type = -1;
	while (++type < 3 && ((h = g_ma_holder.head[type]) || 1))
		while (h != NULL)
		{
			write(1, ((char[3][5]){"TINY", "SMALL", "LARGE"})[type], 5 - !type);
			error = ma_debug_utoabuff((size_t)h, buff, 16, "0123456789ABCDEF");
			write(1, " : 0x0000000000", 15 - error);
			write(1, buff, error);
			write(1, "\n", 1);
			l = h->data;
			while (l != NULL)
			{
				if (l->allocated)
					print_array(l->data, l->size);
				if ((l = ma_next_block(h, type, l, &error)) == NULL && error)
					write(1, "CORRUPTED BLOCK FOUND\n", 16);
			}
			h = h->next;
		}
}
