/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_debug_itoabuff.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/20 17:06:28 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 18:51:22 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

int	ma_debug_utoabuff(unsigned long long i, char buff[65], int base,
					const char *base_str)
{
	int					l;
	unsigned long long	t;

	l = 1;
	t = i;
	while (t /= base)
		++l;
	t = l;
	buff[l] = '\0';
	while (t--)
	{
		buff[t] = base_str[i % base];
		i /= base;
	}
	return (l);
}
