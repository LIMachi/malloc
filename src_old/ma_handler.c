/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_handler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 02:08:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/23 04:21:29 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"

t_ma_handler	*ma_handler(void)
{
	static t_ma_handler	g_ma_handler = {.flags = UNINITIALIZED,

	.tiny_td = {.bloc_size = 0, .blocs_per_page = 0, .pages_per_header = 0,
	.header_size = 0, .largest_size = 0}, .small_td = {.bloc_size = 0,
	.blocs_per_page = 0, .pages_per_header = 0, .header_size = 0,
	.largest_size = 0}, .page_size = 0, .guard_edges = 0, .scribble = 0,
	.tiny = NULL, .small = NULL, .large = NULL, .func = { .new_space = NULL,
	.get_space = NULL, .search_pointer = NULL}, .pages_mapped = 0,
	.pages_writen = 0};
	return (&g_ma_handler);
}
