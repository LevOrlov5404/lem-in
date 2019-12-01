/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 14:39:28 by pnita             #+#    #+#             */
/*   Updated: 2019/11/10 14:39:29 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

void	    clear_after_bfs(t_lem *lem)
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

t_w		    *get_way(t_lem *p)
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

t_w		    *bfs(t_lem *p)
{
	t_r	*queque;
	t_r	*tmp_r;
	t_l	*tmp_link;

	// printf("IN BFS\n");
	queque = p->start;
	queque->steps_in_q = 0;
	queque->was_in_q = 1;
	while (queque)
	{
		// printf("queque = %s\n", queque->name);
		if (queque == p->end)
		{
			p->meet_end = 1;
			break ;
		}
		if (queque->room_in && !queque->room_in->steps_in_q && !queque->moved_inside)
		{
			tmp_r = queque->room_in;
			tmp_r->steps_in_q = queque->steps_in_q;
			tmp_r->prev_in_q = queque;
			add_r_in_queque(queque, tmp_r);
		}
		tmp_link = queque->links;
		if (queque->room_out)
		{
			tmp_r = find_r(p, tmp_link->name)->room_out;
			if (tmp_r && !tmp_r->was_in_q)
			{
				// printf("tmp_r = %s\n", tmp_r->name);
				tmp_r->prev_in_q = queque;
				tmp_r->steps_in_q = queque->steps_in_q + 1;
				add_r_in_queque(queque, tmp_r);
			}
		}
		else
		{
			while (tmp_link)
			{
				if (!tmp_link->not_active)
				{
					tmp_r = find_r(p, tmp_link->name);
					if (!tmp_r->was_in_q)
					{
						// printf("tmp_r = %s\n", tmp_r->name);
						tmp_r->prev_in_q = queque;
						tmp_r->steps_in_q = queque->steps_in_q + 1;
						add_r_in_queque(queque, tmp_r);
					}
				}
				tmp_link = tmp_link->next;
			}
		}
		queque = queque->next_in_q;
	}
	return (get_way(p));
}

void		reverse_way(t_w *way, t_lem *lem)
{
	t_w	*w;
	t_w	*next_w;
	t_l	*tmp_link;
	char *r_name_before;
	t_r	*room_out;

	w = way;
	next_w = w->next;
	tmp_link = find_link(w->r, next_w->r->name);
	tmp_link->not_active = 1;
	r_name_before = w->r->name;
	while (next_w->r != lem->end)
	{
		w = next_w;
		next_w = next_w->next;
		if (w->r->room_in && !ft_strcmp(w->r->name, next_w->r->name)) // try check if it is alreade reverse room, and than not reverse 
			w->r->moved_inside = 1;
		else
		{
			tmp_link = find_link(w->r, next_w->r->name);
			tmp_link->not_active = 1;
		}
		if (!w->r->room_out && !w->r->room_in)
		{
			room_out = create_room(w->r->name, lem); // not create room_out if it already exist
			room_out->links = w->r->links;
			room_out->room_in = w->r;
			w->r->room_out = room_out;
			w->r->links = NULL;
		}
		add_link(w->r, r_name_before); // ? add link because it can be more than one
		r_name_before = w->r->name;
	}
}
