/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:42:51 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/18 01:14:39 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

/*
** typedef size_t
*/

# include <stddef.h>

/*
** see man free, malloc, realloc
*/

void						free(void *ptr);
void						*malloc(size_t size);
void						*realloc(void *ptr, size_t size);

/*
** see https://cdn.intra.42.fr/pdf/pdf/1192/ft_malloc.en.pdf
*/

void						show_alloc_mem(void);

/*
** bonus, see man calloc, reallocf, (GLibc) malloc_usable_size
*/

void						*calloc(size_t nbmem, size_t size);
void						*reallocf(void *ptr, size_t size);
size_t						malloc_usable_size(void *ptr);
#endif
