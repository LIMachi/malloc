/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 18:27:31 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 19:32:15 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/malloc.h"

#include <unistd.h>

/*
** ssize_t write(int fildes, const void *buf, size_t nbyte)
*/

#include <sys/resource.h>

/*
** struct rusage
** int getrusage(int who, struct rusage *r_usage)
*/


static inline int	sif_itoa_buff(int i, char buff[42])
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

static inline void	sif_print_rusage(struct rusage local_rusage)
{
	int		l;
	char	buff[42];

	write(1, "page reclaims: ", 15);
	l = sif_itoa_buff(local_rusage.ru_minflt, buff);
	write(1, buff, l);
	write(1, "\npage faults: ", 14);
	l = sif_itoa_buff(local_rusage.ru_majflt, buff);
	write(1, buff, l);
	write(1, "\n", 1);
}

int					main(void)
{
	struct rusage	local_rusage;
	long			minflt;
	long			majflt;
	char			buff[24];


	// write(1, "before\n", 7);
	getrusage(RUSAGE_SELF, &local_rusage);
	minflt = local_rusage.ru_minflt;
	majflt = local_rusage.ru_majflt;
	// sif_print_rusage(local_rusage);
	{
		int			 i;
		char			*addr;

		i = 0;
		while (i < 1024)
		{
			// write(1, buff, sif_itoa_buff(i, buff));
			// write(1, "\n", 1);
			addr = (char*)malloc(1024);
			addr[0] = 42;
			i++;
		}
	}
	// write(1, "after\n", 6);
	getrusage(RUSAGE_SELF, &local_rusage);
	// sif_print_rusage(local_rusage);
	write(1, "delta\n", 6);
	local_rusage.ru_minflt -= minflt;
	local_rusage.ru_majflt -= majflt;
	sif_print_rusage(local_rusage);
	return (0);
}
