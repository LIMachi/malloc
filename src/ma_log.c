/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ma_log.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/25 10:26:31 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/25 20:54:12 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/*
** nb_arg precise the number of null terminated strings passed to ma_log
*/

MA_PRIVATE size_t	ma_log(const char *function_name, size_t nb_arg, ...)
{
	va_list			va;
	char			*str;
	char			buff[65];

	if (!(g_ma_holder.bonus.flags & FILE_LOG))
		return (0);
	write(g_ma_holder.bonus.log_fd, "[MALLOC_LOG][", 13);
	write(g_ma_holder.bonus.log_fd, buff, ma_debug_utoabuff(
		g_ma_holder.bonus.call_number, buff, 10, "0123456789"));
	write(g_ma_holder.bonus.log_fd, "][", 2);
	write(g_ma_holder.bonus.log_fd, function_name, strlen(function_name));
	write(g_ma_holder.bonus.log_fd, "]: ", 3);
	va_start(va, nb_arg);
	while (nb_arg--)
	{
		str = va_arg(va, char*);
		write(g_ma_holder.bonus.log_fd, str, strlen(str));
	}
	write(g_ma_holder.bonus.log_fd, "\n", 1);
	return (0);
}

MA_PRIVATE size_t	ma_error(const char *function_name, size_t nb_arg, ...)
{
	va_list			va;
	char			*str;
	char			buff[65];

	write(g_ma_holder.bonus.log_fd, "[MALLOC_ERROR][", 15);
	write(g_ma_holder.bonus.log_fd, buff, ma_debug_utoabuff(
		g_ma_holder.bonus.call_number, buff, 10, "0123456789"));
	write(g_ma_holder.bonus.log_fd, "][", 2);
	write(g_ma_holder.bonus.log_fd, function_name, strlen(function_name));
	write(g_ma_holder.bonus.log_fd, "]: ", 3);
	va_start(va, nb_arg);
	while (nb_arg--)
	{
		str = va_arg(va, char*);
		write(g_ma_holder.bonus.log_fd, str, strlen(str));
	}
	write(g_ma_holder.bonus.log_fd, "\n", 1);
	if (g_ma_holder.bonus.flags & EXIT_ON_ERROR)
		exit (0);
	return (0);
}
