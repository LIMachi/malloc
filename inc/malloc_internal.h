/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 01:56:32 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/10 02:50:36 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# include <stddef.h>

/*
** typedef size_t
*/

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

typedef struct						s_ma_header_large
{
	struct s_ma_large_header		*next;
	struct s_ma_large_header		*prev;
	int								status;
	size_t							size;
}									t_ma_header_large;

typedef struct						s_ma_header_small_tiny
{
	struct s_ma_header_small_tiny	*next;
	struct s_ma_header_small_tiny	*prev;
}									t_ma_header_small_tiny;

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
** FINAL_FREE: call free on all allocated memory on program exit
** NO_FREE: disable entirely the calls to free, for testing purpose
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
	FINAL_FREE = 128,
	NO_FREE = 256
}							t_mah_flags;

/*
** flags: see t_mah_flags definition above, default SCRIBBLE
** tiny_bloc: maximum size of a tiny bloc (not including header and guards)
** tiny_page:
** tiny_head_pages:
** small_bloc: maximum size of a small bloc (not including header and guards)
** small_page:
** small_head_pages:
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
	size_t							tiny_bloc;
	size_t							tiny_page;
	size_t							tiny_head_pages;
	size_t							small_bloc;
	size_t							small_head_pages;
	size_t							small_page;
	size_t							page_size;
	size_t							guard_edges;
	char							scribble;
	t_ma_header_small_tiny			*tiny;
	t_ma_header_small_tiny			*small;
	t_ma_header_large				*large;
};

/*
** transform getpagesize in a power representation (ex 4096 == 2p12)
** then cut the power in one third / two third (ex 2p12 == 2p4 * 2p8)
** the low power represent the tiny block, the high power represent the small
** ex for 4096: tiny: 64, small: 256
** it is the most omogenous, but not the most optimised for small allocations
*/

/*
** 4096
** head {		16
** 	prev	8
** 	next	8
** }
** pages_pointers { pages * 8
** 	pages 8
** }
** blocs_tiny (16) { pages * 512
** 	usage 256 * pages
** 	owner 256 * pages
** }
** blocs_small (128) { pages * 64
** 	usage 32 * pages
** 	owner 32 * pages
** }
** total tiny: pages * 520 + 16 == 4096 ~= pages == 7 loss: 440
** total small: pages * 72 + 16 == 4096 ~= pages == 56 loss: 48
** blocs_per_pages = 4096 / BLOC_SIZE
** first alloc = ceil(128 / blocs_per_pages) * pages
** bloc_head = 8 + blocs_per_pages * 2
** pages == (int)(4080 / bloc_head)
** loss == 4080 - pages * bloc_head
** 1024 * malloc(1024) == aprox 5 header plus 256 small pages, 261 pages total
** search of pointer:
** first rule: never dereference a pointer given to free/realloc
** test if pointer is in a page by comparing the pointer and the page itself (if ((size_t)pointer >= (size_t)page[n] && (size_t)pointer < pagesize + (size_t)page[n]))
** since all allocated blocs are aligned on BLOC_SIZE, non aligned pointer must be refused (if (!((size_t)pointer - (size_t)page[n]) % BLOC_SIZE)))
** finally, we can push the security further by testing if previous bloc are allocated to the same group
** since all mmaped pages are never munmaped, pages_pointers are list of up to x pages and eventually NULL terminated (if less than x)
*/

extern struct s_ma_handler			g_ma_handler;

void								malloc_init(void);
void								*ma_search_pointer(const size_t ptr,
												int *type, size_t *index);

#endif
