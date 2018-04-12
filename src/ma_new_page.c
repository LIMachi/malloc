/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_new_page.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/12 15:10:13 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/12 17:00:03 by hmartzol         ###   ########.fr       */
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

t_ma_header_small_tiny	*ma_new_page_tiny(size_t *index)
{
	t_ma_header_small_tiny	*head;

	head = g_ma_handler.tiny;
	while (head != NULL)
	{
		*index = 0;
		while (*index < g_ma_handler.tiny_td.pages_per_header &&
				((void**)head)[2 + *index] != NULL)
			++*index;
		if (*index < g_ma_handler.tiny_td.pages_per_header)
		{
			((void**)head)[2 + *index] = mmap(0, g_ma_handler.page_size,
				PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
			return (head);
		}
		head = head->next;
	}
	if ((head = ma_new_head(g_ma_handler.tiny_td)) == NULL)
		return (NULL);
	head->next = g_ma_handler.tiny;
	g_ma_handler.tiny->prev = head;
	*index = 0;
	return ((g_ma_handler.tiny = head));
}

t_ma_header_small_tiny	*ma_new_page_small(size_t *index)
{
	t_ma_header_small_tiny	*head;

	head = g_ma_handler.small;
	while (head != NULL)
	{
		*index = 0;
		while (*index < g_ma_handler.small_td.pages_per_header &&
				((void**)head)[2 + *index] != NULL)
			++*index;
		if (*index < g_ma_handler.small_td.pages_per_header)
		{
			((void**)head)[2 + *index] = mmap(0, g_ma_handler.page_size,
				PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
			return (head);
		}
		head = head->next;
	}
	if ((head = ma_new_head(g_ma_handler.small_td)) == NULL)
		return (NULL);
	head->next = g_ma_handler.small;
	g_ma_handler.small->prev = head;
	*index = 0;
	return ((g_ma_handler.small = head));
}
