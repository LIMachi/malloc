/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_search_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/09 21:20:41 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/22 16:26:25 by hmartzol         ###   ########.fr       */
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

static inline size_t	sif_search_bloc(t_ma_header_bloc **h, const size_t ptr,
										const t_ma_type_data td, size_t index)
{
	size_t		bloc;
	uint16_t	*blocs;
	uint16_t	id;

	write(1, "sif_search_bloc\n", 16); //DEBUG
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
		return (INVALID_POINTER);
	blocs = (uint16_t*)&(*h)->pages[td.pages_per_header];
	bloc = ((size_t)(*h)->pages[index] - ptr) / td.bloc_size;
	if (!(id = blocs[index * td.blocs_per_page + bloc]))
		index = FREED_POINTER;
	return (!(g_ma_handler.flags & LAZY_ALIGN) && ((((size_t)(*h)->pages[index]
		- ptr) % td.bloc_size) || (bloc > 0 && id == blocs[
		index * td.blocs_per_page + bloc - 1])) ? BAD_ALIGN : index);
}

static inline void		*sif_search_list(const size_t ptr,
										t_ma_header_link *h, size_t *index)
{
	size_t				p;

	*index = 0;
	write(1, "sif_search_list\n", 16); //DEBUG
	while (h != NULL)
	{
		p = ((size_t)h) + sizeof(t_ma_header_link);
		if (p <= ptr && ptr < p + h->size)
		{
			if (!(h->size & USED))
				*index = FREED_POINTER;
			if (!(g_ma_handler.flags & LAZY_ALIGN) && p != ptr)
				*index = BAD_ALIGN;
			if ((ssize_t)*index < 0)
				return (NULL);
			return (h);
		}
		h = h->next;
	}
	*index = INVALID_POINTER;
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

void					*ma_search_pointer_bloc(const size_t ptr, int *type,
												size_t *index)
{
	t_ma_header_bloc	*h;

	write(1, "search_pointer_bloc\n", 20); //DEBUG
	h = g_ma_handler.tiny;
	*type = TINY;
	if ((ssize_t)(*index = sif_search_bloc(&h, ptr, g_ma_handler.tiny_td, 0))
			< 0)
		return (NULL);
	if (h == NULL)
	{
		h = g_ma_handler.small;
		*type = SMALL;
		if ((ssize_t)(*index = sif_search_bloc(&h, ptr, g_ma_handler.small_td,
				0)) < 0)
			return (NULL);
		if (h == NULL)
		{
			*type = LARGE;
			return (sif_search_list(ptr, g_ma_handler.large, index));
		}
	}
	return ((void*)h);
}

/*
** this version will return the page group header that contains the pointer
** or NULL if no page contains it
*/

void					*ma_search_pointer_list(const size_t ptr, int *type,
												size_t *index)
{
	t_ma_header_link	*h;
	void				*p;

	write(1, "search_pointer_list\n", 20); //DEBUG
	h = (t_ma_header_link*)g_ma_handler.tiny;
	*type = TINY;
	while (h != NULL)
	{
		if ((p = sif_search_list(ptr, (t_ma_header_link*)h->data, index)) !=
				NULL || (ssize_t)*index < (ssize_t)INVALID_POINTER)
			return (p);
		h = h->next;
	}
	h = (t_ma_header_link*)g_ma_handler.small;
	*type = SMALL;
	while (h != NULL)
	{
		if ((p = sif_search_list(ptr, (t_ma_header_link*)h->data, index)) !=
				NULL || (ssize_t)*index < (ssize_t)INVALID_POINTER)
			return (p);
		h = h->next;
	}
	*type = LARGE;
	return (sif_search_list(ptr, g_ma_handler.large, index));
}
