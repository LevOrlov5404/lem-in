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

#include "lem_in.h"

int			bfs_part2(t_lem *lem, t_r *queque)
{
	t_r	*tmp_r;

	if (queque == lem->end)
	{
		lem->meet_end = 1;
		return (0);
	}
	if (queque->room_in && !queque->room_in->steps_in_q
			&& !queque->moved_inside)
	{
		tmp_r = queque->room_in;
		tmp_r->steps_in_q = queque->steps_in_q;
		tmp_r->prev_in_q = queque;
		add_r_in_queque(queque, tmp_r);
	}
	return (1);
}

void		bfs_part3(t_lem *lem, t_r *queque, t_l *tmp_link)
{
	t_r	*tmp_r;

	tmp_r = find_r(lem, tmp_link->name)->room_out;
	if (tmp_r && !tmp_r->was_in_q)
	{
		tmp_r->prev_in_q = queque;
		tmp_r->steps_in_q = queque->steps_in_q + 1;
		add_r_in_queque(queque, tmp_r);
	}
}

void		bfs_part4(t_lem *lem, t_r *queque, t_l *tmp_link)
{
	t_r	*tmp_r;

	if (!tmp_link->not_active)
	{
		tmp_r = find_r(lem, tmp_link->name);
		if (!tmp_r->was_in_q)
		{
			tmp_r->prev_in_q = queque;
			tmp_r->steps_in_q = queque->steps_in_q + 1;
			add_r_in_queque(queque, tmp_r);
		}
	}
}

t_w			*bfs(t_lem *lem)
{
	t_r	*queque;
	t_l	*tmp_link;

	queque = lem->start;
	queque->steps_in_q = 0;
	queque->was_in_q = 1;
	while (queque)
	{
		if (!bfs_part2(lem, queque))
			break ;
		tmp_link = queque->links;
		if (queque->room_out)
			bfs_part3(lem, queque, tmp_link);
		else
		{
			while (tmp_link)
			{
				bfs_part4(lem, queque, tmp_link);
				tmp_link = tmp_link->next;
			}
		}
		queque = queque->next_in_q;
	}
	return (get_way(lem));
}
