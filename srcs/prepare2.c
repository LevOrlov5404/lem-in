/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 22:51:18 by pnita             #+#    #+#             */
/*   Updated: 2019/12/01 22:51:20 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

int				tmp_not_in_next(t_r *tmp, t_r *next)
{
	t_l	*check_wl;

	check_wl = next->way_links;
	while (check_wl)
	{
		if (!ft_strcmp(check_wl->name, tmp->name))
		{
			check_wl->not_active = 1;
			return (0);
		}
		check_wl = check_wl->next;
	}
	return (1);
}

t_w_with_len	**create_ways(int n)
{
	t_w_with_len	**ways;
	int				i;

	ways = (t_w_with_len**)ft_memalloc(sizeof(t_w_with_len*) * n);
	i = 0;
	while (i < n)
		ways[i++] = (t_w_with_len*)ft_memalloc(sizeof(t_w_with_len));
	return (ways);
}

void			paste_way_link_at_r(t_w *way)
{
	t_r	*tmp_r;
	t_r	*next_r;

	while (way->next)
	{
		tmp_r = way->r;
		next_r = way->next->r;
		add_way_link((tmp_r->room_in ? tmp_r->room_in : tmp_r), next_r->name);
		way = way->next;
	}
}

void			paste_way(t_lem *lem, t_w *way)
{
	t_r	*tmp_r;
	t_r *next_r;
	t_l	*tmp_wl;

	tmp_r = way->next->r;
	while (tmp_r != lem->end)
	{
		tmp_wl = tmp_r->way_links;
		while (tmp_wl && tmp_wl->not_active)
			tmp_wl = tmp_wl->next;
		next_r = find_r(lem, tmp_wl->name);
		if (tmp_not_in_next(tmp_r, next_r))
		{
			add_way(way, next_r);
			tmp_r = next_r;
		}
		else
			tmp_wl->not_active = 1;
	}
}

void			clear_after_change_ways(t_lem *lem, t_w_with_len **ways,
		int n_ways, t_w *new_way)
{
	int	i;
	t_w	*tmp_w;

	i = 0;
	while (i < n_ways)
	{
		tmp_w = ways[i++]->way;
		while (tmp_w->r != lem->end)
		{
			delete_way_link(tmp_w->r);
			tmp_w = tmp_w->next;
		}
	}
	tmp_w = new_way;
	while (tmp_w->r != lem->end)
	{
		delete_way_link(tmp_w->r);
		tmp_w = tmp_w->next;
	}
}
