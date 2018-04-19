/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_search_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:20:41 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/19 05:02:43 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

#include <unistd.h>

/*
** ssize_t
*/

#include <stdint.h>

/*
** uint16_t
*/

static inline size_t				sif_search_bloc(t_ma_header_bloc **h,
													const size_t ptr,
													const t_ma_type_data td,
													size_t index)
{
	size_t		bloc;
	uint16_t	*blocs;
	uint16_t	id;

	while (*h != NULL && !(index = 0))
	{
		while (index < td.pages_per_header && (*h)->pages[index] != NULL &&
				!(ptr >= (size_t)(*h)->pages[index] && ptr <
				(size_t)(*h)->pages[index] + g_ma_handler.page_size))
			++index;
		if (index < td.pages_per_header && (*h)->pages[index] != NULL &&
				ptr >= (size_t)(*h)->pages[index] && ptr <
				(size_t)(*h)->pages[index] + g_ma_handler.page_size)
			break ;
		*h = (*h)->next;
	}
	if (*h == NULL)
		return ((size_t)-1);
	blocs = (uint16_t*)&(*h)->pages[td.pages_per_header];
	bloc = ((size_t)(*h)->pages[index] - ptr) / td.bloc_size;
	if (!(id = blocs[index * td.blocs_per_page + bloc]))
		index = -2;
	return (!(g_ma_handler.flags & LAZY_ALIGN) && ((((size_t)(*h)->pages[index]
		- ptr) % td.bloc_size) || (bloc > 0 && id == blocs[
		index * td.blocs_per_page + bloc - 1])) ? (size_t)-3 : index);
}

static inline void				*sif_search_list(const size_t ptr,
													size_t *index)
{
	t_ma_header_link	*h;
	size_t				p;

	h = g_ma_handler.large;
	*index = 0;
	while (h != NULL)
	{
		p = ((size_t)h) + sizeof(t_ma_header_link);
		if (p <= ptr && ptr < p + h->size)
		{
			if (!(h->size & USED))
				*index = -2;
			if (!(g_ma_handler.flags & LAZY_ALIGN) && p != ptr)
				*index = -3;
			if ((ssize_t)*index < 0)
				return (NULL);
			return (h);
		}
		h = h->next;
	}
	*index = -1;
	return (NULL);
}

/*
** return the page header in which ptr might be located. type will be set to 1
** if it is a tiny page, 2 for small and 3 for large. index will be set to the
** index of the page in the header (undefined for large or for LIST mode)
** if NULL returned and index == -1, no valid pointer was found
** if NULL returned and index == -2, pointer is already free
** if NULL returned and index == -3, an allocation englobing the pointer was
** found, but LAZY_ALIGN is not activated (in most case,
** equivalent to index == -1)
*/

void								*ma_search_pointer_bloc(const size_t ptr,
												int *type,
												size_t *index)
{
	t_ma_header_bloc	*h;

	h = g_ma_handler.tiny;
	*type = 1;
	if ((ssize_t)(*index = sif_search_bloc(&h, ptr, g_ma_handler.tiny_td, 0))
			< 0)
		return (NULL);
	if (h == NULL)
	{
		h = g_ma_handler.small;
		*type = 2;
		if ((ssize_t)(*index = sif_search_bloc(&h, ptr, g_ma_handler.small_td,
				0)) < 0)
			return (NULL);
		if (h == NULL)
		{
			*type = 3;
			return (sif_search_list(ptr, index));
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
