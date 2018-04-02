/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 19:17:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/02 19:24:11 by hmartzol         ###   ########.fr       */
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
# define BONUS				0

struct s_ma_page;

typedef struct				s_ma_header
{
	size_t					size;
	size_t					flags;
	struct s_ma_header		*previous;
	struct s_ma_header		*next;
	struct s_ma_page		*page;
}							t_ma_header;

typedef struct				s_ma_page
{
	void					*data;
	struct s_ma_page		*previous;
	struct s_ma_page		*next;
}							t_ma_page;

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
