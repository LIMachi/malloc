/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:42:51 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 18:46:27 by hmartzol         ###   ########.fr       */
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

void	free(void *ptr) __attribute__((visibility("default")));
void	*malloc(size_t size) __attribute__((visibility("default")));
void	*realloc(void *ptr, size_t size) __attribute__((visibility("default")));

/*
** See https://cdn.intra.42.fr/pdf/pdf/1192/ft_malloc.en.pdf
*/

void	show_alloc_mem(void) __attribute__((visibility("default")));

/*
** Bonus, see man calloc, reallocf (BSD), malloc_usable_size (GLibc)
*/

void	*calloc(size_t nbmem,
		size_t size) __attribute__((visibility("default")));
void	*reallocf(void *ptr,
		size_t size) __attribute__((visibility("default")));
size_t	malloc_usable_size(void *ptr) __attribute__((visibility("default")));

/*
** Bonus, differ from GLibc's malloc_trim. Will agggressivelly munmap all unused
** pools except the first of TINY and SMALL, will return the number of pages
** released
*/

size_t	malloc_trim(void) __attribute__((visibility("default")));

#endif
