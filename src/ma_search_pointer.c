/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_search_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:20:41 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/19 04:16:09 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

#include <stdint.h>

/*
** uint16_t
*/

static inline size_t				sif_sub_search(t_ma_header_bloc **h,
													const size_t ptr,
													const t_ma_type_data td)
{
	size_t		n;
	size_t		bloc;
	uint16_t	*blocs;

	n = 0;
	while (*h != NULL)
	{
		n = 0;
		while (n < td.pages_per_header && (*h)->pages[n] != 0 &&
				!(ptr >= (size_t)(*h)->pages[n] && ptr < (size_t)(*h)->pages[n]
				+ g_ma_handler.page_size))
			++n;
		if (n < td.pages_per_header && (*h)->pages[n] != 0 &&
				ptr >= (size_t)(*h)->pages[n] && ptr < (size_t)(*h)->pages[n] +
				g_ma_handler.page_size)
			break ;
		*h = (*h)->next;
	}
	blocs = (uint16_t*)&(*h)->pages[td.pages_per_header];
	bloc = ((size_t)(*h)->pages[n] - ptr) / td.bloc_size;
	return ((*h != NULL && !(g_ma_handler.flags & LAZY_ALIGN) && (
		(((size_t)(*h)->pages[n] - ptr) % td.bloc_size) ||
		(bloc > 0 && blocs[n * td.blocs_per_page + bloc - 1] ==
			blocs[n * td.blocs_per_page + bloc]))) ? (size_t)-1 : n);
}

static inline void				*sif_large_search(const size_t ptr,
													size_t *index)
{
	t_ma_header_link	*h;
	size_t				p;

	h = g_ma_handler.large;
	while (h != NULL)
	{
		p = ((size_t)h) + sizeof(t_ma_header_link);
		if (p <= ptr && ptr < p + h->size)
		{
			if (!(g_ma_handler.flags & LAZY_ALIGN) && p != ptr)
			{
				*index = -1;
				return (NULL);
			}
			return (h);
		}
		h = h->next;
	}
	return (NULL);
}

/*
** return the page header in which ptr might be located. type will be set to 1
** if it is a tiny page, 2 for small and 3 for large. index will be set to the
** index of the page in the header (undefined for large or for LIST mode)
** if NULL returned and index == -1, an allocation englobing the pointer was
** found, but LAZY_ALIGN is not activated
*/

void								*ma_search_pointer_bloc(const size_t ptr,
												int *type,
												size_t *index)
{
	t_ma_header_bloc	*h;

	h = g_ma_handler.tiny;
	*type = 1;
	*index = 0;
	if ((*index = sif_sub_search(&h, ptr, g_ma_handler.tiny_td)) == (size_t)-1)
		return (NULL);
	if (h == NULL)
	{
		h = g_ma_handler.small;
		*type = 2;
		if ((*index = sif_sub_search(&h, ptr, g_ma_handler.small_td)) ==
				(size_t)-1)
			return (NULL);
		if (h == NULL)
		{
			*type = 3;
			return (sif_large_search(ptr, index));
		}
	}
	return ((void*)h);
}

/*
** this version will return the page group header that contains the pointer
** or NULL if no page contains it
*/

void								*ma_search_pointer_list(const size_t ptr,
												int *type,
												size_t *index)
{
	(void)ptr;
	(void)type;
	(void)index;
	return (NULL);
}
