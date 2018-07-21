/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memcpy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 19:50:58 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/21 19:53:20 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** typedef size_t
*/

# include <stddef.h>

/*
** FIXME: memcpy basique et stupide, a remplacer
*/

void	*memcpy(void *dest, const void *src, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		((char*)dest)[i] = ((char*)src)[i];
	return (dest);
}
