/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/03 13:05:58 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/14 23:52:50 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

# include <malloc_defines.h>

# include <stddef.h>

/*
** typedef size_t
*/

/*
** values for MA_MODE: MA_MODE_BLOC (use bloc allocation, information of
** allocation is separated from data)
** MA_MODE_LIST (use linked list style allocation, information and data are next
** to each other)
** MA_MODE_LIST is much faster but less secure (dereference pointer content)
** MA_MODE_BLOC is slower but less prone to error on miswrinting in invalid
** addresses
** if in BONUS mode (define BONUS to not be 0), MA_MODE_DEFAULT can be
** overwriten at run time by the env entry: MALLOC_MODE=LIST/BLOC/DEFAULT
*/

# define MA_MODE_DEFAULT	MA_MODE_BLOC

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
