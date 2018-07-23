/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:42:51 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/23 07:14:35 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

/*
** typedef size_t
*/

# include <stddef.h>

/*
** See man free, malloc, realloc
*/

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

/*
** See https://cdn.intra.42.fr/pdf/pdf/1192/ft_malloc.en.pdf
*/

void	show_alloc_mem(void);

/*
** Bonus, see man calloc, reallocf (BSD), malloc_usable_size (GLibc)
*/

void	*calloc(size_t nbmem, size_t size);
void	*reallocf(void *ptr, size_t size);
size_t	malloc_usable_size(void *ptr);

/*
** Bonus, differ from GLibc's malloc_trim. Will agggressivelly munmap all unused
** pools except the first of TINY and SMALL, will return the number of pages
** released
*/

size_t	malloc_trim(void);

#endif
