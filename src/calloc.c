/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 03:59:32 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/23 04:21:56 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_internal.h"

#if BONUS

# include <errno.h>

void	*calloc(size_t nbmem, size_t size)
{
	char	*ptr;
	size_t	i;

	printf("calloc(%zu, %zu)\n", nbmem, size);
	pthread_mutex_lock(&g_ma_mutex);
	if (nbmem * size < nbmem)
	{
		errno = EOVERFLOW;
		return (NULL);
	}
	if ((ptr = malloc(nbmem * size)) == NULL)
		return (NULL);
	i = -1;
	while (++i < nbmem * size)
		ptr[i] = 0;
	pthread_mutex_unlock(&g_ma_mutex);
	printf("quitting calloc\n");
	return (ptr);
}

#else

void	*calloc(size_t nbmem, size_t size)
{
	char	*ptr;
	size_t	i;

	if (nbmem * size < nbmem)
		return (NULL);
	if ((ptr = malloc(nbmem * size)) == NULL)
		return (NULL);
	i = -1;
	while (++i < nbmem * size)
		ptr[i] = 0;
	return (ptr);
}

#endif
