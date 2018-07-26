/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 22:05:30 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/26 13:53:47 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc_internal.h>

/*
** cas 1: nouvelle taille est équivalente a ancienne taille: rien
** cas 2: nouvelle taille est plus petite que l'ancienne, mais delta inferieur a
** 		sizeof(t_ma_link): rien (impossible de découper un sous block)
** cas 3: nouvelle taille plus petite et delta supérieur ou égal a
** 		sizeof(t_ma_link): création d'un sous bloc égal à ancienne taille moins
** 		nouvelle taille après le bloc realloc
** cas 4: la nouvelle taille est plus grande que l'ancienne, mais il n'y a pas
** 		de bloc libre après: malloc
** cas 5: la nouvelle taille est plus grande et la taille du bloc actuel plus le
** 		suivant est trop petite: malloc
** cas 6: la nouvelle taille plus la taille du bloc libre suivant peut contenir
** 		uniquement le nouveau bloc, on arrondit la taille
** cas 7: la nouvelle taille plus la taille du bloc libre suivant peut contenir
** 		la taille demandée plus un autre bloc: on les crée
** cas 1-3:
** 		(size <= f->found->size)
** 		cas 1, 2: (cas 3 testé en premier, ce qui rend ce cas inutile)
** 			(f->found->size - size < sizeof(t_ma_link))
** 		cas 3: (else)
** 			(f->found->size - size >= sizeof(t_ma_link))
** cas 4-7: (else) (sif_realloc1)
** 		(size > f->found->size)
** 		cas 4, 5:
** 			(f->after == NULL || !f->after->allocated || f->after->size
** 				+ f->found->size + sizeof(t_ma_link) < size)
** 		cas 6: (else)
** 			(f->found->size + f->after->size + sizeof(t_ma_link) - size
** 				< sizeof(t_ma_link))
** 		cas 7: (else)
** 			(f->found->size + f->after->size + sizeof(t_ma_link) - size
** 				>= sizeof(t_ma_link))
*/

static inline void	*sif_realloc1(t_ma_found_link *f, size_t size)
{
	t_ma_link	*tmp;

	if (f->after == NULL || !f->after->allocated
		|| f->after->size + f->found->size + sizeof(t_ma_link) < size)
	{
		ma_log("realloc", 1, "call forwarded to malloc (pool swap)");
		--g_ma_holder.bonus.call_number;
		if ((tmp = malloc(size - g_ma_holder.bonus.guard_edges * 2)) == NULL)
			return (NULL);
		memcpy(tmp, f->found->data + g_ma_holder.bonus.guard_edges,
				(f->found->size < size ? f->found->size : size)
				- g_ma_holder.bonus.guard_edges * 2);
		ma_free(f);
		return (tmp + g_ma_holder.bonus.guard_edges);
	}
	else if (f->found->size + f->after->size + sizeof(t_ma_link)
			- size < sizeof(t_ma_link))
		f->found->size += f->after->size + sizeof(t_ma_link);
	else
	{
		tmp = (t_ma_link*)&f->found->data[size];
		tmp->size = f->found->size + f->after->size - size;
		tmp->allocated = 0;
		f->found->size = size;
	}
	return (f->found->data);
}

static inline void	*sif_realloc0(t_ma_found_link *f, size_t size)
{
	t_ma_link	*tmp;
	void		*out;

	if (size <= f->found->size)
	{
		if (f->found->size - size >= sizeof(t_ma_link))
		{
			tmp = (t_ma_link*)&f->found->data[size];
			tmp->size = f->found->size - size - sizeof(t_ma_link);
			tmp->allocated = 0;
			f->found->size = size;
		}
		return (f->found->data);
	}
	out = sif_realloc1(f, size) + g_ma_holder.bonus.guard_edges;

	return (out);
}

static inline int	sif_realloc2(void *p, size_t size, void **out)
{
	// char	buff0[65];
	// char	buff1[65];

	*out = NULL;
	pthread_mutex_lock(&g_ma_mutex);
	if (!g_ma_holder.initialized)
		ma_init();
	++g_ma_holder.bonus.call_number;
	if (p == NULL)
	{
		*out = malloc(size);
		return (1);
	}
	if (size == 0)
	{
		free(p);
		return (1);
	}
	return (0);
/*
	if (g_ma_holder.bonus.flags & (ALLOC_LOG | FREE_LOG))
	{
		ma_debug_utoabuff((size_t)p, buff0, 16, "0123456789ABCDEF");
		ma_debug_utoabuff(size, buff1, 10, "0123456789");
		ma_log(4, "call to realloc on 0x", buff0, " with a new size of ", buff1,
			" bytes");
		if (p == NULL)
			ma_log(1, "realloc call forwarded to malloc (NULL pointer)");
		else if (size == 0)
			ma_log(1, "realloc call forwarded to free (null size)");
	}
*/
}

MA_PUBLIC void		*realloc(void *p, size_t size)
{
	t_ma_found_link	f;
	void			*out;

	if (sif_realloc2(p, size, &out))
		return (out);
	if (size == 0 || !ma_validate_pointer(p - g_ma_holder.bonus.guard_edges, &f)
				|| !f.found->allocated)
		return (NULL);
	if (ma_categorize((size += g_ma_holder.bonus.guard_edges * 2)) != f.type
		|| (ma_categorize(size) == MA_T_LARGE && f.found->size < size))
	{
		ma_log("realloc", 1, "call forwarded to malloc (pool swap)");
		--g_ma_holder.bonus.call_number;
		if ((out = malloc(size - g_ma_holder.bonus.guard_edges * 2)) != NULL)
		{
			memcpy(out, f.found->data + g_ma_holder.bonus.guard_edges,
				(f.found->size < size ? f.found->size : size)
				- g_ma_holder.bonus.guard_edges * 2);
			ma_free(&f);
		}
	}
	else
		out = sif_realloc0(&f, size);
	pthread_mutex_unlock(&g_ma_mutex);
	return (out);
}
