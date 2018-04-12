/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_search_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:20:41 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/12 20:23:58 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

static inline size_t				sif_sub_search(char **h,
											const size_t ptr,
											const size_t nb_pages)
{
	size_t	*pages;
	size_t	n;

	while (*h != NULL)
	{
		n = 0;
		pages = ((size_t**)h)[2];
		while (n < nb_pages && pages[n] != 0 &&
				!(ptr >= pages[n] && ptr < pages[n] + g_ma_handler.page_size))
			++n;
		if (n < nb_pages && pages[n] != 0 &&
				ptr >= pages[n] && ptr < pages[n] + g_ma_handler.page_size)
			break ;
		*h = (char*)((t_ma_header_small_tiny*)*h)->next;
	}
	return (n);
}

/*
** return the page header in which ptr might be located. type will be set to 1
** if it is a tiny page, 2 for small and 3 for large. index will be set to the
** index of the page in the header (undefined for large)
*/

inline void							*ma_search_pointer(const size_t ptr,
												int *type,
												size_t *index)
{
	char	*h;

	h = (char*)g_ma_handler.tiny;
	*type = 1;
	*index = sif_sub_search(&h, ptr, g_ma_handler.tiny_td.pages_per_header);
	if (h == NULL)
	{
		h = (char*)g_ma_handler.small;
		*type = 2;
		*index = sif_sub_search(&h, ptr,
			g_ma_handler.small_td.pages_per_header);
		if (h == NULL)
		{
			h = (char*)g_ma_handler.large;
			*type = 3;
			while (h != NULL && ((size_t)h) + sizeof(t_ma_header_large) != ptr)
				h = (char*)((t_ma_header_large*)h)->next;
		}
	}
	return ((void*)h);
}
