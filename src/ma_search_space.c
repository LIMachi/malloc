/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_search_space.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 15:02:37 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/12 23:52:49 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

#include <stdint.h>

/*
** uint16_t
*/

/*
** will search a space large enough to contain size and reserv it
*/

static inline uint16_t	sif_get_new_id(void **head, t_ma_type_data td)
{
	size_t		page;
	size_t		bloc;
	uint8_t		tmp[256];
	uint16_t	id;

	id = -1;
	while (++id < 256)
		tmp[id] = 0;
	page = -1;
	while (++page < td.pages_per_header)
		if (head[2 + page] != NULL && (bloc = -1))
			while (++bloc < td.blocs_per_page)
			{
				id = ((uint16_t*)&head[2 + td.pages_per_header])
					[page * td.blocs_per_page + bloc];
				tmp[id >> 8] |= id & 255;
			}
	id = 0;
	while (++id != 0)
		if (!(tmp[id >> 8] & (id & 255)))
			return (id);
	return (0);
}

static inline void		*sif_get_space(size_t nb_blocs, t_ma_type_data td,
										void **head, size_t st[2])
{
	uint16_t	tmp;
	size_t		contiguous_blocs;

	contiguous_blocs = 0;
	if (st[1] < td.blocs_per_page)
		while (contiguous_blocs < nb_blocs &&
				contiguous_blocs + st[1] < td.blocs_per_page &&
				!(((uint16_t*)&head[2 + td.pages_per_header])
					[st[0] * td.blocs_per_page + st[1] + contiguous_blocs]))
			++contiguous_blocs;
	if (contiguous_blocs == nb_blocs && (tmp = sif_get_new_id(head, td)))
	{
		while (contiguous_blocs--)
			((uint16_t*)&head[2 + td.pages_per_header])
				[st[0] * td.blocs_per_page + st[1] + contiguous_blocs] = tmp;
		return ((void*)(((char*)head[2 + st[0]]) + st[1] * td.bloc_size));
	}
	return (NULL);
}

void					*ma_get_space(size_t nb_blocs, t_ma_type_data td,
										void **head)
{
	size_t		page;
	size_t		first_bloc;
	void		*tmp;

	while (head != NULL && (page = -1))
	{
		while (++page < td.pages_per_header && (first_bloc = -1))
		{
			if (head[2 + page] == NULL)
				continue ;
			first_bloc = 0;
			while (first_bloc < td.blocs_per_page)
			{
				while (first_bloc < td.blocs_per_page &&
						((uint16_t*)&head[2 + td.pages_per_header])
							[page * td.blocs_per_page + first_bloc])
					++first_bloc;
				if ((tmp = sif_get_space(nb_blocs, td, head,
						(size_t[2]){page, first_bloc})) != NULL)
					return (tmp);
			}
		}
		head = (void**)((t_ma_header_small_tiny*)head)->next;
	}
	return (NULL);
}
