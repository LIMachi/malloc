/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_new_page.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 15:10:13 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/22 16:18:31 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

#include <sys/mman.h>

/*
** void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
** int munmap(void *addr, size_t len)
*/

/*
** create a new page and try to insert it in a head, if it can't, it uses
** ma_new_head instead
** return the page_head and index in which the new page was inserted
*/

void	*ma_new_page(void **head, const t_ma_type_data td,
					size_t *index)
{
	t_ma_header_bloc	*tmp;

	write(1, "ma_new_page\n", 12);
	tmp = *head;
	while (tmp != NULL)
	{
		*index = 0;
		while (*index < td.pages_per_header &&
				((void**)tmp)[2 + *index] != NULL)
			++*index;
		if (*index < td.pages_per_header)
		{
			((void**)tmp)[2 + *index] = mmap(0, g_ma_handler.page_size,
				PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
			return (tmp);
		}
		tmp = tmp->next;
	}
	return (ma_new_head_bloc(head, td, index));
}
