/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old_method.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 19:17:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/12 11:00:10 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OLD_METHOD_H
# define OLD_METHOD_H

# include <stddef.h>

/*
** typedef size_t
*/

# define HINT_TINY_SIZE		64
# define HINT_SMALL_SIZE	1024

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

# endif

# pragma pack(push, 1)

/*
** sizeof(t_ma_header_small_tiny) == 4
** size: size of the bloc following this header
** previous: position of the previous bloc, relative to this header
** flags: bitwise flag, used to tell if memory is allocated
**   0b01: free
**   0b10: unused
** size and previous does not include guard or header
*/

typedef struct				s_ma_header_small_tiny
{
	unsigned long long		size : 15;
	unsigned long long		previous : 15;
	unsigned long long		flags : 2;
}							t_ma_header_small_tiny;

/*
** sizeof(t_ma_page) == 32 / 16
** size: the total size of the page, including this header
** last_block: position (relative to this header) of the last bloc in the page
** flags: bitwise flag for type and status
**   0b01: large page
**   0b10: free page (no bloc/only free blocs)
** previous: pointer to another page, or NULL if this page is the first of the
**   chain
** next: pointer to another page, or NULL if this page is the last of the chain
*/

# if __SIZEOF_POINTER__ == 8

struct						s_ma_page_64
{
	unsigned long long		size : 63;
	unsigned long long		last_block : 63;
	unsigned long long		flags : 2;
	struct s_ma_page		*previous;
	struct s_ma_page		*next;
};

#  define S_MA_PAGE		struct s_ma_page_64

# elif __SIZEOF_POINTER__ == 4

struct						s_ma_page_32
{
	unsigned long long		size : 31;
	unsigned long long		last_block : 31;
	unsigned long long		flags : 2;
	struct s_ma_page		*previous;
	struct s_ma_page		*next;
};

#  define S_MA_PAGE		struct s_ma_page_32

# else
#  error "Invalid pointer size"
# endif

typedef S_MA_PAGE			t_ma_page;

# pragma pack(pop)

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
** tiny_size: maximum size of a tiny bloc (not including header and guards)
** tiny_zone: size of allocated zones (including header and guards, zones can
**   contain at least 128 bloc)
** small_size: maximum size of a small bloc (not including header and guards)
** small_zone: size of allocated zones (including header and guards, zones can
**   contain at least 128 bloc)
** page_size: value returned by getpagesize()
** guard_edges: size of edges to add to blocs, default 0
** scribble: character to use has scribble on unalocated memory, default 0x42
** tiny: pointer to the first zone of tiny blocs
** small: pointer to the first zone of small blocs
** large: pointer to the first zone of large blocs, default NULL
*/

struct						s_ma_handler
{
	t_mah_flags				flags;
	size_t					tiny_size;
	size_t					tiny_zone;
	size_t					small_size;
	size_t					small_zone;
	size_t					page_size;
	size_t					guard_edges;
	char					scribble;
	t_ma_page				*tiny;
	t_ma_page				*small;
	t_ma_page				*large;
};

extern struct s_ma_handler	g_ma_handler;

# if BONUS

extern pthread_mutex_t		g_ma_mutex;

# endif

/*
** internal function to initialise the structure 'g_ma_handler' and to
** pre-allocate 128 tiny and 128 small blocs
*/

void						malloc_init(void);

#endif
