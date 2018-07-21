/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 21:43:03 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/21 20:21:51 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <unistd.h>

MA_PUBLIC void	*malloc(size_t size)
{
	int			type;
	t_ma_link	*l;

	if (!g_ma_holder.initialized)
		ma_init();
	type = ma_categorize(size);
	if ((l = ma_get_space(size, type)) == NULL)
	{
		if (ma_new_pool(size, type) == NULL)
			return (NULL);
		l = ma_get_space(size, type);
	}
	return (l->data);
}
