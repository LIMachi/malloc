/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 01:56:32 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/26 03:59:29 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# include "malloc.h"

# ifndef BONUS
#  define BONUS				0
# endif

# include <unistd.h> //DEBUG
# include <stdio.h> //DEBUG

# if BONUS

#  include <pthread.h>

/*
** pthread_mutex_t
** int pthread_mutex_lock(pthread_mutex_t *mutex)
** int pthread_mutex_unlock(pthread_mutex_t *mutex)
*/

extern pthread_mutex_t		g_ma_mutex;

# endif

/*
** MA_MODE_LIST / MA_MODE_BLOC (large only)
** size is the size of the pool (including the pool header itself)
** or the size of the allocated data (not including the header)
** (page/large) size is a multiple of getpagesize and should be able to andle up
** to PREALLOC alloc of maximum size of the type of the page (tiny/small)
** in the case of large bloc, the page group contain no lists and the data is
** directly accesible via the fake array data. otherwise, use data as the first
** link of the list
*/

typedef struct				s_ma_header_link
{
	struct s_ma_header_link	*next;
	struct s_ma_header_link	*prev;
	size_t					size;
	char					data[0];
}							t_ma_header_link;

/*
** MA_MODE_BLOC
** common header for small and tiny group page, note that after pages (which is
** variable in length) is another array of the form uint16_t usage[page][bloc]
** accessing usage:
** uint16_t *usage = (uint16_t*)&head->pages[td.pages_per_header];
** usage[page * td.blocs_per_page + bloc]
** struct							s_pseudo_header
** {
** 		t_ma_header_small_tiny		*next;
** 		t_ma_header_small_tiny		*prev;
** 		void						*pages[pages_per_header];
** 		uint16_t					usage[blocs_per_page * pages_per_header];
** };
*/

typedef struct				s_ma_header_bloc
{
	struct s_ma_header_bloc	*next;
	struct s_ma_header_bloc	*prev;
	void					*pages[0];
}							t_ma_header_bloc;

# define _											//FIXME: not norm compliant

/*
** UNINITIALIZED: default state
** INITIALIZED: prevent reinitialisation
** SCRIBBLE: signal to free to scribble over the unallocated memory with the
**   character ma_handler()->scribble set by the env 'MALLOC_SCRIBBLE='
** GUARD_EDGES: signal to alloc functions to add (ma_handler()->guard_edges)
**   bytes around allocated memory. ma_handler()->guard_edges is set by the env
**   'MALLOC_GUARD_EDGES='
** all following values are flags set by the env prefixing them with 'MALLOC_'
** example: to set ALLOC_LOG, add to env the entry 'MALLOC_ALLOC_LOG'
** ALLOC_LOG: print information on calls to alloc functions
** FREE_LOG: print information on calls to free
** HEXDUMP: extend the output format of 'show_alloc_mem'
** FRAGMENTED: disable defragmentation of freed memory, for testing purpose
** NO_FREE: disable entirely the calls to free, for testing purpose
** the last flag is set by MALLOC_MODE=LIST/DEFAULT/BLOC
** MODE_LIST: if set, MA_MODE_LIST will be used over MA_MODE_BLOC
*/

typedef enum				e_mah_flags
{
	UNINITIALIZED =			0,
	INITIALIZED =			1 << 0,
	SCRIBBLE =				1 << 1,
	GUARD_EDGES =			1 << 2,
	ALLOC_LOG =				1 << 3,
	FREE_LOG =				1 << 4,
	HEXDUMP =				1 << 5,
	FRAGMENTED =			1 << 6,
	NO_FREE =				1 << 7,
	MODE_LIST =				1 << 8,
	LAZY_ALIGN =			1 << 9,
	NO_UNMAP =				1 << 10,
	EXIT_ON_ERROR =			1 << 11,
	FILE_LOG =				1 << 12
}							t_mah_flags;

/*
** mostly explicit
** header_size is the total size of the header (plus padding) and is a multiple
** of getpagesize()
** largest_size is the largest size of an allocation cutable in pages
** some of those information are discarded in MA_MODE_LIST
** header_size is used as pool size in LIST mode
*/

typedef struct				s_ma_type_data
{
	size_t					bloc_size;
	size_t					blocs_per_page;
	size_t					pages_per_header;
	size_t					header_size;
	size_t					largest_size;
}							t_ma_type_data;

/*
** collection of pointers to function used to switch between bloc and list mode
*/

typedef struct				s_ma_func
{
	void				*(*new_space)(void **, const t_ma_type_data, size_t *);
	void				*(*get_space)(size_t, const t_ma_type_data, void**);
	void				*(*search_pointer)(const size_t, int*, size_t*);
}							t_ma_func;

/*
** flags: see t_mah_flags definition above, default SCRIBBLE
** tiny_td, small_td: look t_ma_type_data
** page_size: value returned by getpagesize()
** guard_edges: size of edges to add to blocs, default 0
** scribble: character to use has scribble on unalocated memory, default 0x42
** tiny: pointer to the first zone of tiny blocs
** small: pointer to the first zone of small blocs
** large: pointer to the first zone of large blocs, default NULL
*/

typedef struct				s_ma_handler
{
	t_mah_flags				flags;
	t_ma_type_data			tiny_td;
	t_ma_type_data			small_td;
	size_t					page_size;
	size_t					guard_edges;
	char					scribble;
	void					*tiny;
	void					*small;
	t_ma_header_link		*large;
	t_ma_func				func;
	size_t					pages_mapped;
	size_t					pages_writen;
	int						log_fd;
}							t_ma_handler;

// extern struct s_ma_handler	g_ma_handler;

void						handler_container(void);

t_ma_handler				*ma_handler(void);

int							malloc_init(size_t index);

// void						*calloc(size_t nbmem, size_t size);

void						aggressive_munmap(void);

void						*ma_search_pointer_bloc(const size_t ptr,
										int *type, size_t *index);
void						*ma_search_pointer_list(const size_t ptr,
										int *type, size_t *index);

void						*ma_new_page(void **head,
								const t_ma_type_data td, size_t *index);
void						*ma_new_head_list(void **head,
								const t_ma_type_data td, size_t *index);
void						*ma_new_head_bloc(void **head,
								const t_ma_type_data td, size_t *index);

void						*ma_get_space_bloc(size_t size,
									t_ma_type_data td, void **head);
void						*ma_get_space_list(size_t size,
									t_ma_type_data td, void **head);
#endif
