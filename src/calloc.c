/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 20:33:02 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 13:52:58 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

void	*calloc(size_t nmemb, size_t size)
{
	char	*out;
	size_t	i;
	char	buff0[65];
	char	buff1[65];

	pthread_mutex_lock(&g_ma_mutex);
	if (!g_ma_holder.initialized)
		ma_init();
	pthread_mutex_unlock(&g_ma_mutex);
	ma_debug_utoabuff(nmemb, buff0, 10, "0123456789");
	ma_debug_utoabuff(size, buff1, 10, "0123456789");
	ma_log("calloc", 4, "call to calloc with ", buff0 ,", ", buff1);
	if (nmemb * size < size)
	{
		ma_error("calloc", 1, "invalid calloc total size");
		return (NULL);
	}
	ma_log("calloc", 1, "call to calloc forwarded to malloc");
	out = malloc(nmemb * size);
	i = -1;
	while (++i < nmemb * size)
		out[i] = 0;
	return (out);
}
