/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3++.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 18:35:33 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/22 15:35:00 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include "../inc/malloc.h"

#define M (1024 * 1024)

void print(char *s)
{
	write(1, s, strlen(s));
}

int main(void)
{
	char	*addr1;
	char	*addr2;
	char	*addr3;

	addr1 = (char*)malloc(16*M);
	strcpy(addr1, "Bonjours\n");
	show_alloc_mem();
	print(addr1);
	addr2 = (char*)malloc(16*M);
	show_alloc_mem();
	addr3 = (char*)realloc(addr1, 128*M);
	show_alloc_mem();
	addr3[127*M] = 42;
	print(addr3);
	return (0);
}
