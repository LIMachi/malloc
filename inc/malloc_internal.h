/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:43:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/22 16:47:21 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# define DEBUG

# include <malloc.h>

# if !(defined _WIN32 || defined __CYGWIN__) && __GNUC__ >= 4
#  define MA_PRIVATE __attribute__ ((visibility ("hidden")))
#  define MA_PUBLIC __attribute__ ((visibility ("default")))
# else
#  define MA_PRIVATE
#  define MA_PUBLIC
# endif

typedef struct					s_ma_link
{
	size_t						size : __SIZEOF_SIZE_T__ * 8 - 1;
	size_t						allocated : 1;
	char						data[0];
}								t_ma_link;

typedef struct					s_ma_head
{
	struct s_ma_head			*next;
	struct s_ma_head			*prev;
	t_ma_link					data[0];
}								t_ma_head;

typedef struct					s_ma_found_link
{
	int							type;
	t_ma_head					*head;
	t_ma_link					*before;
	t_ma_link					*found;
	t_ma_link					*after;
}								t_ma_found_link;

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

# ifdef DEBUG
typedef struct					s_ma_debug
{
	size_t						pool_count;
	size_t						alloc_count;
}								t_ma_debug;
# endif

typedef struct					s_ma_holder
{
	int							initialized;
	size_t						page_size;
	size_t						pool_size;
	t_ma_type_data				td[3];
	t_ma_head					*head[3];
# ifdef BONUS
	t_ma_bonus					bonus;
# endif
# ifdef DEBUG
	t_ma_debug					debug;
# endif
}								t_ma_holder;

t_ma_holder						g_ma_holder;

/*
** void ma_init(void):	initialise elements of g_ma_holder
*/

void							ma_init(void);

/*
** int ma_categorize(size_t *size):	round up size and return in which pool this
** 									size fit
*/

int								ma_categorize(size_t size);

/*
**
*/

t_ma_link						*ma_get_space(size_t nb, int type);

/*
**
*/

t_ma_head						*ma_new_pool(size_t size, int type);

/*
** DEBUG
*/

int								ma_debug_utoabuff(unsigned long long i,
								char buff[65], int base, const char *base_str);

/*
**
*/

int								ma_validate_pointer(void *ptr,
													t_ma_found_link *f);

/*
**
*/

void							ma_free(t_ma_found_link *f);

/*
**
*/

void							*memcpy(void *dest, const void *src, size_t n);

/*
**
*/

void							*calloc(size_t nmemb, size_t size);

#endif
