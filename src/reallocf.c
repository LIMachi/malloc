/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 15:59:29 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 14:16:36 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

static inline int	reallocf1(void *ptr, size_t size, void **out)
{
	char	buff0[65];
	char	buff1[65];

	*out = NULL;
	pthread_mutex_lock(&g_ma_mutex);
	if (!g_ma_holder.initialized)
		ma_init();
	if (g_ma_holder.bonus.flags & (ALLOC_LOG | FREE_LOG))
	{
		ma_debug_utoabuff((size_t)ptr, buff0, 16, "0123456789ABCDEF");
		ma_debug_utoabuff(size, buff1, 10, "0123456789");
		ma_log("reallocf", 5, "reallocf call on 0x", buff0,
							"with a new size of ", buff1, " bytes");
	}
	if (size == 0)
	{
		ma_log("reallocf", 1, "reallocf call forwarded to free (null size)");
		pthread_mutex_unlock(&g_ma_mutex);
		free(ptr);
		return (1);
	}
	ma_log("reallocf", 1, "reallocf call forwarded to realloc");
	pthread_mutex_unlock(&g_ma_mutex);
	return (0);
}

MA_PUBLIC void		*reallocf(void *ptr, size_t size)
{
	void	*out;

	if (reallocf1(ptr, size, &out))
		return (out);
	if ((out = realloc(ptr, size)) == NULL)
	{
		pthread_mutex_lock(&g_ma_mutex);
		ma_error("reallocf", 1,
			"previous realloc call returned NULL, call forwarded to NULL");
		pthread_mutex_unlock(&g_ma_mutex);
		free(ptr);
	}
	return (out);
}
