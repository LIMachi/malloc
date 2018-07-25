/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 19:49:37 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/25 20:47:09 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>
#include <string.h>

MA_PRIVATE void	ma_free(t_ma_found_link *f)
{
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
}

MA_PUBLIC void	free(void *ptr)
{
	t_ma_found_link	f;
	char			buff[65];

	if (!g_ma_holder.initialized)
		ma_init();
	++g_ma_holder.bonus.call_number;
	if (g_ma_holder.bonus.flags & FREE_LOG)
	{
		ma_debug_utoabuff((size_t)ptr, buff, 16, "0123456789ABCDEF");
		ma_log("free", 2, "free pointer 0x", buff);
	}
	if (ptr == NULL)
		return ;
	if (!ma_validate_pointer(ptr - g_ma_holder.bonus.guard_edges, &f))
		return ;
	if (!f.found->allocated)
	{
		ma_debug_utoabuff((size_t)ptr, buff, 16, "0123456789ABCDEF");
		ma_error("free", 2, "trying to free unalocated pointer 0x", buff);
		return ;
	}
	ma_free(&f);
}
