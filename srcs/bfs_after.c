/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs_after.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 16:49:55 by pnita             #+#    #+#             */
/*   Updated: 2019/12/02 16:49:56 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void		clear_after_bfs(t_lem *lem)
{
	t_r	*r;
	t_r	*next_r;

	r = lem->start;
	next_r = r->next_in_q;
	while (next_r)
	{
		r->steps_in_q = 0;
		r->was_in_q = 0;
		r->next_in_q = NULL;
		r->prev_in_q = NULL;
		r = next_r;
		next_r = next_r->next_in_q;
	}
	r->steps_in_q = 0;
	r->was_in_q = 0;
	r->next_in_q = NULL;
	r->prev_in_q = NULL;
	lem->meet_end = 0;
}

t_w			*get_way(t_lem *p)
{
	t_r	*queque;
	t_r	*tmp_r;
	t_w *way;

	if (!p->meet_end)
		return (NULL);
	way = create_way(p->end);
	queque = p->end;
	while (queque != p->start)
	{
		tmp_r = queque->prev_in_q;
		push_to_begin_w(&way, tmp_r);
		queque = tmp_r;
	}
	clear_after_bfs(p);
	return (way);
}

void		reverse_way_step(t_lem *lem, t_w *w, t_w *next_w, t_l *tmp_link)
{
	t_r	*room_out;

	if (w->r->room_in && !ft_strcmp(w->r->name, next_w->r->name))
		w->r->moved_inside = 1;
	else
	{
		tmp_link = find_link(w->r, next_w->r->name);
		tmp_link->not_active = 1;
	}
	if (!w->r->room_out && !w->r->room_in)
	{
		room_out = create_room(w->r->name, lem);
		room_out->links = w->r->links;
		room_out->room_in = w->r;
		w->r->room_out = room_out;
		w->r->links = NULL;
	}
}

void		reverse_way(t_w *way, t_lem *lem)
{
	t_w		*w;
	t_w		*next_w;
	t_l		*tmp_link;
	char	*r_name_before;

	w = way;
	next_w = w->next;
	tmp_link = find_link(w->r, next_w->r->name);
	tmp_link->not_active = 1;
	r_name_before = w->r->name;
	while (next_w->r != lem->end)
	{
		w = next_w;
		next_w = next_w->next;
		reverse_way_step(lem, w, next_w, tmp_link);
		add_link(w->r, r_name_before);
		r_name_before = w->r->name;
	}
}
