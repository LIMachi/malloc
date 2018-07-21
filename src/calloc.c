/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 20:33:02 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/21 20:35:45 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

void	*calloc(size_t nmemb, size_t size)
{
	char	*out;
	size_t	i;

	if (nmemb * size < size)
		return (NULL);
	out = malloc(nmemb * size);
	i = -1;
	while (++i < nmemb * size)
		out[i] = 0;
	return (out);
}
