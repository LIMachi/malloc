/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 20:24:37 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 16:26:08 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <unistd.h>

static inline void	print_array(t_ma_link *l)
{
	char				buff[65];
	static const char	base[16] = "0123456789ABCDEF";
	size_t				len;
	size_t				data;
	size_t				size;

	data = (size_t)l->data + g_ma_holder.bonus.guard_edges;
	size = l->size - g_ma_holder.bonus.guard_edges * 2;
	len = ma_debug_utoabuff((size_t)data, buff, 16, base);
	write(1, "0x0000000000", 12 - len);
	write(1, buff, len);
	len = ma_debug_utoabuff((size_t)data + size, buff, 16, base);
	write(1, " - 0x0000000000", 15 - len);
	write(1, buff, len);
	write(1, " : ", 3);
	write(1, buff, ma_debug_utoabuff(size, buff, 10, base));
	write(1, " bytes\n", 7);
	if (g_ma_holder.bonus.flags & HEXDUMP)
		hexdump(l->data, l->size);
}

static inline void	print_pool(t_ma_head *h, int type)
{
	size_t	ptr_size;
	char	buff[65];

	write(1, ((char[3][5]){"TINY", "SMALL", "LARGE"})[type], 5 - !type);
	ptr_size = ma_debug_utoabuff((size_t)h, buff, 16, "0123456789ABCDEF");
	write(1, " : 0x0000000000", 15 - ptr_size);
	write(1, buff, ptr_size);
	write(1, "\n", 1);
}

MA_PUBLIC void	show_alloc_mem(void)
{
	t_ma_head			*h;
	t_ma_link			*l;
	int					type;
	int					error;

	type = -1;
	pthread_mutex_lock(&g_ma_mutex);
	while (++type < 3 && ((h = g_ma_holder.head[type]) || 1))
		while (h != NULL)
		{
			print_pool(h, type);
			l = h->data;
			while (l != NULL)
			{
				if (l->allocated || g_ma_holder.bonus.flags & SHOW_UNALLOCATED)
					print_array(l);
				if ((l = ma_next_block(h, type, l, &error)) == NULL && error)
					write(1, "CORRUPTED BLOCK FOUND\n", 16);
			}
			h = h->next;
		}
	pthread_mutex_unlock(&g_ma_mutex);
}
