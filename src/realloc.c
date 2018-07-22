/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/17 22:05:30 by hmartzol          #+#    #+#             */
/*   Updated: 2018/07/22 16:47:10 by hmartzol         ###   ########.fr       */
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
		if ((tmp = malloc(size)) == NULL)
			return (NULL);
		memcpy(tmp, f->found->data,
				f->found->size < size ? f->found->size : size);
		ma_free(f);
		return (tmp);
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
	return (sif_realloc1(f, size));
}

MA_PUBLIC void		*realloc(void *ptr, size_t size)
{
	t_ma_found_link	f;
	int				type;
	void			*out;

	if (ptr == NULL)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ma_validate_pointer(ptr, &f) || !f.found->allocated)
		return (NULL); //invalid pointer
	type = ma_categorize(size);
	if (type != f.type || (type == MA_T_LARGE && f.found->size < size))
	{
		if ((out = malloc(size)) != NULL)
		{
			memcpy(out, ptr, f.found->size < size ? f.found->size : size);
			ma_free(&f);
		}
		return (out);
	}
	return (sif_realloc0(&f, size));
}
