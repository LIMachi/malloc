/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 18:27:31 by hmartzol          #+#    #+#             */
/*   Updated: 2018/04/23 02:18:15 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include <unistd.h>

void	test1(void)
{
	write(1, "1\n", 2);
}

void	test2(void)
{
	write(1, "2\n", 2);
}

int main(void)
{
	int		i;
	char	*addr;

	write(1, "----------------- in main -------------\n", 40);
	i = 0;
	while (i < 1024)
	{
		// addr = (char*)malloc(1024);
		// addr[0] = 42;
		i++;
	}
	return (0);
}
