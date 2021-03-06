/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:43:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 19:18:10 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# include <malloc.h>

# include <pthread.h>

# include <unistd.h>

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
	size_t						minimum_size;
	size_t						maximum_size;
	size_t						pool_size;
}								t_ma_type_data;

typedef enum					e_ma_flags
{
	UNINITIALIZED = 0,
	INITIALIZED = 1 << 0,
	SCRIBBLE = 1 << 1,
	GUARD_EDGES = 1 << 2,
	ALLOC_LOG = 1 << 3,
	FREE_LOG = 1 << 4,
	HEXDUMP = 1 << 5,
	FRAGMENTED = 1 << 6,
	NO_FREE = 1 << 7,
	MODE_LIST = 1 << 8,
	LAZY_ALIGN = 1 << 9,
	NO_UNMAP = 1 << 10,
	EXIT_ON_ERROR = 1 << 11,
	FILE_LOG = 1 << 12,
	SHOW_UNALLOCATED = 1 << 13
}								t_ma_flags;

# define MA_T_TINY				0
# define MA_T_SMALL				1
# define MA_T_LARGE				2

# define MA_DEFAULT_POOL_SIZE	128

typedef struct					s_ma_bonus
{
	t_ma_flags					flags;
	size_t						guard_edges;
	char						scribble;
	size_t						pages_mapped;
	size_t						pages_writen;
	int							log_fd;
	size_t						call_number;
	size_t						hexdump_len;
}								t_ma_bonus;

pthread_mutex_t					g_ma_mutex;

typedef struct					s_ma_debug
{
	size_t						pool_count;
	size_t						alloc_count;
}								t_ma_debug;

typedef struct					s_ma_holder
{
	int							initialized;
	size_t						page_size;
	size_t						pool_size;
	t_ma_type_data				td[3];
	t_ma_head					*head[3];
	t_ma_bonus					bonus;
	t_ma_debug					debug;
}								t_ma_holder;

t_ma_holder						g_ma_holder;

void							ma_init(
						void) __attribute__((visibility("hidden")));

int								ma_categorize(
						size_t size) __attribute__((visibility("hidden")));
t_ma_link						*ma_get_space(size_t nb,
						int type) __attribute__((visibility("hidden")));
t_ma_head						*ma_new_pool(size_t size,
						int type) __attribute__((visibility("hidden")));
int								ma_debug_utoabuff(unsigned long long i,
						char buff[65], int base,
					const char *base_str) __attribute__((visibility("hidden")));
int								ma_validate_pointer(void *ptr,
					t_ma_found_link *f) __attribute__((visibility("hidden")));
int								ma_free(
	t_ma_found_link *f) __attribute__((visibility("hidden")));
void							*memcpy(void *dest, const void *src,
						size_t n) __attribute__((visibility("hidden")));
t_ma_link						*ma_next_block(t_ma_head *h, int type,
						t_ma_link *l,
						int *error) __attribute__((visibility("hidden")));
t_ma_link						*ma_malloc(size_t size,
						int type) __attribute__((visibility("hidden")));
size_t							ma_log(const char *function_name,
					size_t nb_arg, ...) __attribute__((visibility("hidden")));
size_t							ma_error(const char *function_name,
					size_t nb_arg, ...) __attribute__((visibility("hidden")));
void							hexdump(void *data,
					size_t size) __attribute__((visibility("hidden")));

#endif
