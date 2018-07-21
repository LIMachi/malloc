/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_debug_itoabuff.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/20 17:06:28 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/21 20:23:58 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

MA_PRIVATE int	ma_debug_itoabuff(int i, char buff[42])
{
	int l;
	int t;

	l = 1;
	t = i;
	while (t /= 10)
		++l;
	t = l;
	buff[l] = '\0';
	while (t--)
	{
		buff[t] = i % 10 + '0';
		i /= 10;
	}
	return (l);
}
