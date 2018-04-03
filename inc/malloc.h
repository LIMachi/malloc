/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 19:17:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/03 11:56:56 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H

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
** size and previous does not include guard or header
*/

/*
** sizeof(t_ma_header_small_tiny) == 4
*/

typedef struct				s_ma_header_small_tiny
{
	unsigned long long		size : 15;
	unsigned long long		previous : 15;
	unsigned long long		flags : 2;
}							t_ma_header_small_tiny;

/*
** sizeof(t_ma_page) == 32 / 16
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

# pragma pack(pop)

typedef S_MA_PAGE			t_ma_page;

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

void						free(void *ptr);
void						*malloc(size_t size);
void						*realloc(void *ptr, size_t size);
void						show_alloc_mem(void);

void						malloc_init(void);
#endif
