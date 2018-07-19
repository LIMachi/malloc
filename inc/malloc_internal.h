/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:43:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/19 15:11:21 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# include <malloc.h>

# pragma pack(push, 1)

typedef struct					s_ma_link
{
	size_t						size : __SIZEOF_SIZE_T__ * 8 - 1;
	size_t						allocated : 1;
	char						data[0];
}								t_ma_link;

typedef struct					s_ma_head
{
	struct s_ma_head			*next;
	t_ma_link					data[0];
}								t_ma_head;

# pragma pack(pop)

/*
** note: this 2 structures are used for all 3 types of pool, a large is
** basically a pool of only one element
*/

typedef struct					s_ma_type_data
{
	size_t						minimum_size;	//in bytes
	size_t						maximum_size;	//in bytes
												// -1 for large (pseudo infinite/unused)
	size_t						pool_size;		//ceil((sizeof(t_ma_header_link) + POOL_SIZE * (maximum_size + sizeof(t_ma_header_link))) / pagesize) * pagesize
												//0 for large (adaptative/unused)
}								t_ma_type_data;

typedef enum					e_ma_flags
{
	UNINITIALIZED =				0,
	INITIALIZED =				1 << 0,
	SCRIBBLE =					1 << 1,
	GUARD_EDGES =				1 << 2,
	ALLOC_LOG =					1 << 3,
	FREE_LOG =					1 << 4,
	HEXDUMP =					1 << 5,
	FRAGMENTED =				1 << 6,
	NO_FREE =					1 << 7,
	MODE_LIST =					1 << 8,
	LAZY_ALIGN =				1 << 9,
	NO_UNMAP =					1 << 10,
	EXIT_ON_ERROR =				1 << 11,
	FILE_LOG =					1 << 12
}								t_ma_flags;

# define MA_T_TINY				0
# define MA_T_SMALL				1
# define MA_T_LARGE				2

# define MA_DEFAULT_POOL_SIZE	128

# ifdef BONUS

typedef struct					s_ma_bonus
{
	t_ma_flags					flags;
	size_t						guard_edges;
	char						scribble;
	size_t						pages_mapped;
	size_t						pages_writen;
	int							log_fd;
}								t_ma_bonus;

pthread_mutex_t					g_ma_mutex;

# endif

typedef struct					s_ma_holder
{
	int							initialized;
	size_t						page_size;
	size_t						pool_size;
	t_ma_type_data				td[3];
	t_ma_header_link			*head[3];
# ifdef BONUS
	t_ma_bonus					bonus;
# endif
}								t_ma_holder;

t_ma_holder						g_ma_holder = {
	.initialized = 0,
	.page_size = 0,
	.pool_size = MA_DEFAULT_POOL_SIZE,
	.td = {
		{
			.minimum_size = 0,
			.maximum_size = 0,
			.pool_size = 0,
		},
		{
			.minimum_size = 0,
			.maximum_size = 0,
			.pool_size = 0,
		},
		{
			.minimum_size = 0,
			.maximum_size = 0,
			.pool_size = 0,
		}
	},
	.head = {NULL, NULL, NULL}
# ifdef BONUS
	, .bonus = {
		.flags = 0,
		.guard_edges = 0,
		.scribble = '\0',
		.pages_mapped = 0,
		.pages_writen = 0,
		.log_fd = -1
	}
# endif
};

/*
** void ma_init(void):	initialise elements of g_ma_holder
*/

void							ma_init(void);

/*
** int ma_categorize(size_t *size):	round up size and return in which pool this
** 									size fit
*/

int								ma_categorize(size_t *size);

#endif
