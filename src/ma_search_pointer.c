/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_search_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:20:41 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/16 17:12:23 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

static inline size_t				sif_sub_search(t_ma_header_small_tiny **h,
											const size_t ptr,
											const size_t nb_pages)
{
	size_t	n;

	while (*h != NULL)
	{
		n = 0;
		while (n < nb_pages && (*h)->pages[n] != 0 &&
				!(ptr >= (size_t)(*h)->pages[n] && ptr < (size_t)(*h)->pages[n]
				+ g_ma_handler.page_size))
			++n;
		if (n < nb_pages && (*h)->pages[n] != 0 &&
				ptr >= (size_t)(*h)->pages[n] && ptr < (size_t)(*h)->pages[n] +
				g_ma_handler.page_size)
			break ;
		*h = (*h)->next;
	}
	return (n);
}

/*
** return the page header in which ptr might be located. type will be set to 1
** if it is a tiny page, 2 for small and 3 for large. index will be set to the
** index of the page in the header (undefined for large or for LIST mode)
*/

void								*ma_search_pointer_bloc(const size_t ptr,
												int *type,
												size_t *index)
{
	t_ma_header_small_tiny	*h;
	t_ma_header_large		*l;

	h = g_ma_handler.tiny;
	*type = 1;
	*index = sif_sub_search(&h, ptr, g_ma_handler.tiny_td.pages_per_header);
	if (h == NULL)
	{
		h = g_ma_handler.small;
		*type = 2;
		*index = sif_sub_search(&h, ptr,
			g_ma_handler.small_td.pages_per_header);
		if (h == NULL)
		{
			l = g_ma_handler.large;
			*type = 3;
			while (l != NULL && ((size_t)l) + sizeof(t_ma_header_large) != ptr)
				l = l->next;
			return ((void*)l);
		}
	}
	return ((void*)h);
}

void								*ma_search_pointer_list(const size_t ptr,
												int *type,
												size_t *index)
{
	(void)ptr;
	(void)type;
	(void)index;
	return (NULL);
}
