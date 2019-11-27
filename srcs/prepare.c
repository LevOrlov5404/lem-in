/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preleme.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 14:28:52 by pnita             #+#    #+#             */
/*   Updated: 2019/11/10 14:28:55 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

t_lem			*create_lem()
{
	t_lem	*lem;
	int	i;

	lem = (t_lem*)ft_memalloc(sizeof(t_lem));
	lem->r = (t_r**)ft_memalloc(sizeof(t_r*) * ROOMS_SIZE);
	return (lem);
}

size_t		hash(char *input)
{
	size_t	hash_num;

	hash_num = 1;
	while (*input)
	{
		hash_num = hash_num * 3 + *(input++);
	}
	return hash_num % ROOMS_SIZE;
}

t_r			*create_room(char *name, t_lem *lem)
{
	t_r		*r;

	r = (t_r*)ft_memalloc(sizeof(t_r));
	r->name = ft_strdup(name);
	if (lem->is_start)
		lem->start = r;
	else if (lem->is_end)
		lem->end = r;
	return (r);
}

void		add_same_num_room(t_r *r, char *name, t_lem *lem)
{
	t_r	*tmp_r;
	
	tmp_r = r;
	while (tmp_r->same_num)
		tmp_r = tmp_r->same_num;
	tmp_r->same_num = create_room(name, lem);
}

t_r			*find_r(t_lem *lem, char *name)
{
	t_r		*tmp_r;

	tmp_r = lem->r[hash(name)];
	while (tmp_r && ft_strcmp(tmp_r->name, name))
		tmp_r = tmp_r->same_num;
	return (tmp_r);
}
