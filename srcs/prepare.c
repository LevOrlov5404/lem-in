/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 14:28:52 by pnita             #+#    #+#             */
/*   Updated: 2019/11/10 14:28:55 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

t_p			*create_par()
{
	t_p	*par;
	int	i;

	par = (t_p*)ft_memalloc(sizeof(t_p));
	par->r_len = ROOMS_SIZE;
	par->r = (t_r**)ft_memalloc(sizeof(t_r*) * par->r_len);
	return (par);
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

t_r			*create_room(char *name, t_p *par)
{
	t_r		*r;

	r = (t_r*)ft_memalloc(sizeof(t_r));
	r->name = ft_strdup(name);
	if (par->is_start)
		par->start = r;
	if (par->is_end)
		par->end = r;
	return (r);
}

void		add_same_num_room(t_r *r, char *name, t_p *par)
{
	t_r	*tmp_r;
	
	tmp_r = r;
	while (tmp_r->same_num)
		tmp_r = tmp_r->same_num;
	tmp_r->same_num = create_room(name, par);
}

t_r			*find_r(t_p *p, char *name)
{
	t_r		*tmp_r;

	tmp_r = p->r[hash(name)];
	while (tmp_r && ft_strcmp(tmp_r->name, name))
		tmp_r = tmp_r->same_num;
	return (tmp_r);
}
