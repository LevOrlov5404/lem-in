/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   way.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 14:38:10 by pnita             #+#    #+#             */
/*   Updated: 2019/11/10 14:38:12 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

void		add_r_in_queque(t_r *queque, t_r *r_to_add)
{
	while (queque->next_in_q)
		queque = queque->next_in_q;
	queque->next_in_q = r_to_add;
	r_to_add->was_in_q = 1;
}

t_w		    *create_way(t_r *r)
{
	t_w	*way;

	way = (t_w*)ft_memalloc(sizeof(t_w));
	way->r = r;
	return (way);
}

void	    push_to_begin_w(t_w **way, t_r *r)
{
	t_w	*tmp;

	if (!*way)
		*way = create_way(r);
	else
	{
		tmp = create_way(r);
		tmp->next = *way;
		*way = tmp;
	}
}

void	    add_way(t_w *way, t_r *r)
{
	t_w	*tmp;

	if (!way)
		way = create_way(r);
	else
	{
		tmp = way;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = create_way(r);
	}
}

int			len_of_way(t_w *way)
{
	int	len;

	len = 0;
	while (way)
	{
		way = way->next;
		++len;
	}
	return (len - 1);
}
