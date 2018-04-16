/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 01:56:32 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/16 17:11:24 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# include <malloc.h>

# ifndef BONUS
#  define BONUS				0
# endif

# if BONUS

#  include <pthread.h>

/*
** pthread_mutex_t
** int pthread_mutex_lock(pthread_mutex_t *mutex)
** int pthread_mutex_unlock(pthread_mutex_t *mutex)
*/

extern pthread_mutex_t				g_ma_mutex;

# endif

/*
** MA_MODE_LIST
** total_size is the size of the pool (including the pool header itself)
** total_size is a multiple of getpagesize and should be able to andle up to
** PREALLOC alloc of maximum size of the type of the page (tiny/small)
** in the case of large bloc, the page group contain no lists and the data is
** directly accesible via the fake array data. otherwise, use data as the first
** link of the t_ma_header_pool_link list
*/

typedef struct						s_ma_header_pool
{
	struct s_ma_header_pool			*next;
	struct s_ma_header_pool			*prev;
	size_t							total_size;
	char							data[0];
}									t_ma_header_pool;

/*
** MA_MODE_LIST
** alternative bloc header for MA_MODE_LIST, the lowest significant bit of
** size is used as free flag, size will always be treated as a multiple of 2
** prev is the size of the last allocation, 0 means this link is the first
** position of the header + size might be equal to the last byte of the pool, if
** so, this design the current link as the last
*/

typedef struct						s_ma_header_pool_link
{
	struct s_ma_header_pool_link	*next;
	struct s_ma_header_pool_link	*prev;
	size_t							size;
	char							data[0];
}									t_ma_header_pool_link;

/*
** MA_MODE_BLOC
** similar to t_ma_header_pool, explicitly used for large in MA_MODE_BLOC
*/

typedef struct						s_ma_header_large
{
	struct s_ma_header_large		*next;
	struct s_ma_header_large		*prev;
	size_t							size;
	char							data[0];
}									t_ma_header_large;

/*
** MA_MODE_BLOC
** common header for small and tiny group page, note that after pages (which is
** variable in length) is another array of the form uint16_t usage[page][bloc]
** struct							s_pseudo_header
** {
** 		t_ma_header_small_tiny		*next;
** 		t_ma_header_small_tiny		*prev;
** 		void						*pages[pages_per_header];
** 		uint16_t					usage[pages_per_header][blocs_per_page];
** };
*/

typedef struct						s_ma_header_small_tiny
{
	struct s_ma_header_small_tiny	*next;
	struct s_ma_header_small_tiny	*prev;
	void							*pages[0];
}									t_ma_header_small_tiny;

# define _																		//FIXME: not norm compliant

/*
** UNINITIALIZED: default state
** INITIALIZED: prevent reinitialisation
** SCRIBBLE: signal to free to scribble over the unallocated memory with the
**   character g_ma_handler.scribble set by the env 'MALLOC_SCRIBBLE='
** GUARD_EDGES: signal to alloc functions to add (g_ma_handler.guard_edges)
**   bytes around allocated memory. g_ma_handler.guard_edges is set by the env
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
	UNINITIALIZED = 0,
	INITIALIZED = 1,
	SCRIBBLE = 2,
	GUARD_EDGES = 4,
	ALLOC_LOG = 8,
	FREE_LOG = 16,
	HEXDUMP = 32,
	FRAGMENTED = 64,
	NO_FREE = 128,
	MODE_LIST = 256
}							t_mah_flags;

/*
** mostly explicit
** header_size is the total size of the header (plus padding) and is a multiple
** of getpagesize()
** largest_size is the largest size of an allocation cutable in pages
** some of those information are discarded in MA_MODE_LIST
** header_size is used as pool size in LIST mode
*/

typedef struct						s_ma_type_data
{
	size_t							bloc_size;
	size_t							blocs_per_page;
	size_t							pages_per_header;
	size_t							header_size;
	size_t							largest_size;
}									t_ma_type_data;

/*
** collection of pointers to function used to switch between bloc and list mode
*/

typedef struct						s_ma_func
{
	void						*(*new_head)(t_ma_type_data);
	void						*(*get_space)(size_t, t_ma_type_data, void**);
	void						*(*search_pointer)(const size_t, int*, size_t*);
}									t_ma_func;

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

struct								s_ma_handler
{
	t_mah_flags						flags;
	t_ma_type_data					tiny_td;
	t_ma_type_data					small_td;
	size_t							page_size;
	size_t							guard_edges;
	char							scribble;
	t_ma_header_small_tiny			*tiny;
	t_ma_header_small_tiny			*small;
	t_ma_header_large				*large;
	t_ma_func						func;
};

extern struct s_ma_handler			g_ma_handler;

int									malloc_init(void);
void								*ma_search_pointer_bloc(const size_t ptr,
												int *type, size_t *index);
void								*ma_search_pointer_list(const size_t ptr,
												int *type, size_t *index);
void								*ma_new_page_tiny(size_t *index);
void								*ma_new_page_small(size_t *index);
void								*ma_new_head_list(t_ma_type_data td);
void								*ma_new_head_bloc(t_ma_type_data td);
void								*ma_get_space_bloc(size_t size,
											t_ma_type_data td, void **head);
void								*ma_get_space_list(size_t size,
											t_ma_type_data td, void **head);
#endif
