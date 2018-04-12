/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/03 13:05:58 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/10 03:01:58 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <stddef.h>

/*
** typedef size_t
*/

/*
** see man free, malloc, realloc
*/

void						free(void *ptr);
void						*malloc(size_t size);
void						*realloc(void *ptr, size_t size);

/*
** print the actually used memory zones of this library, the format can be
** changed if define BONUS is set to 1 (-DBONUS=1 at compilation time) and
** env has an entry named 'MALLOC_HEXDUMP' (no value is required)
*/

void						show_alloc_mem(void);

#endif
