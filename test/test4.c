/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 18:37:10 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 13:31:50 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"
#include <unistd.h>
#include <string.h>

void print(char *s)
{
	write(1, s, strlen(s));
}

int main()
{
	char	*addr;

	addr = malloc(16);
	free(NULL);
	free((void *)addr + 5);
	free(addr);
	free(addr);
	if (realloc((void *)addr + 5, 10) == NULL)
		print("Bonjours\n");
}
