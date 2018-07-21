/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 15:59:29 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/21 20:22:52 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

MA_PUBLIC void	*reallocf(void *ptr, size_t size)
{
	void	*out;

	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if ((out = realloc(ptr, size)) == NULL)
		free(ptr);
	return (out);
}
