/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 19:50:58 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/22 16:46:06 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

#include <unistd.h>

/*
** FIXME: memcpy basique et stupide, a remplacer
*/

MA_PRIVATE void	*memcpy(void *dest, const void *src, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		((char*)dest)[i] = ((char*)src)[i];
	return (dest);
}
