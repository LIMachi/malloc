/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hexdump.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/26 16:17:42 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 18:51:11 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <unistd.h>
#include <ctype.h>

static inline size_t	sif_utoahexbuff_pad(size_t u, char buff[17], size_t pad)
{
	size_t	i;

	i = -1;
	while (++i < pad)
		buff[i] = '0';
	buff[i] = '\0';
	while (i--)
	{
		buff[i] = "0123456789ABCDEF"[u & 0xF];
		u >>= 4;
	}
	return (pad);
}

static inline void		sif_printables(size_t data, size_t size,
									size_t mem_line)
{
	size_t	c;

	write(1, "  |", 3);
	c = -1;
	while (++c < 16)
		if (mem_line + c < data + size && mem_line + c >= data)
			if (isprint(*(char*)(mem_line + c)))
				write(1, (char*)(mem_line + c), 1);
			else
				write(1, ".", 1);
		else
			write(1, " ", 1);
	write(1, "|\n", 2);
}

static inline size_t	sif_hexdump(void *data, size_t *size)
{
	if (*size > g_ma_holder.bonus.hexdump_len)
		*size = g_ma_holder.bonus.hexdump_len;
	write(1, "0x****************  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E "
	"0F  |****************|\n--------------------------------------------------"
	"-------------------|----------------|\n", 176);
	return (((size_t)data) & ~0xFull);
}

void					hexdump(void *data, size_t size)
{
	char	buff[17];
	size_t	mem_line;
	size_t	c;

	mem_line = sif_hexdump(data, &size);
	while (mem_line < (size_t)data + size)
	{
		write(1, "0x", 2);
		write(1, buff, sif_utoahexbuff_pad(mem_line, buff, 16));
		write(1, " ", 1);
		c = -1;
		while (++c < 16)
		{
			write(1, " ", 1);
			if (mem_line + c < (size_t)data + size
					&& mem_line + c >= (size_t)data)
				write(1, buff, sif_utoahexbuff_pad(
					*(char*)(mem_line + c), buff, 2));
			else
				write(1, "  ", 2);
		}
		sif_printables((size_t)data, size, mem_line);
		mem_line += 16;
	}
}
