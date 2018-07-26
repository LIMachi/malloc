/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 19:49:37 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 18:52:29 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <string.h>

int		ma_free(t_ma_found_link *f)
{
	if (g_ma_holder.bonus.flags & NO_FREE)
	{
		if (g_ma_holder.bonus.flags & FREE_LOG)
			ma_log("ma_free", 1, "call to free ignored (MALLOC_NO_FREE)");
		return (0);
	}
	f->found->allocated = 0;
	if (f->before != NULL && !f->before->allocated)
	{
		f->before->size += f->found->size + sizeof(t_ma_link);
		f->found = f->before;
	}
	if (f->after != NULL && !f->after->allocated)
	{
		f->found->size += f->after->size + sizeof(t_ma_link);
		f->after = f->found;
	}
	if (g_ma_holder.bonus.flags & SCRIBBLE)
		memset(f->found->data, g_ma_holder.bonus.scribble, f->found->size);
	return (0);
}

void	free(void *ptr)
{
	t_ma_found_link	f;
	char			buff[65];

	pthread_mutex_lock(&g_ma_mutex);
	if (!g_ma_holder.initialized)
		ma_init();
	++g_ma_holder.bonus.call_number;
	if (g_ma_holder.bonus.flags & FREE_LOG)
	{
		ma_debug_utoabuff((size_t)ptr, buff, 16, "0123456789ABCDEF");
		ma_log("free", 2, "free pointer 0x", buff);
	}
	if (!ptr || !ma_validate_pointer(ptr - g_ma_holder.bonus.guard_edges, &f))
	{
		pthread_mutex_unlock(&g_ma_mutex);
		return ;
	}
	if (!f.found->allocated)
	{
		ma_debug_utoabuff((size_t)ptr, buff, 16, "0123456789ABCDEF");
		ma_error("free", 2, "trying to free unalocated pointer 0x", buff);
	}
	else
		ma_free(&f);
	pthread_mutex_unlock(&g_ma_mutex);
}
