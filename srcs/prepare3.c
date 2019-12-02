/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 16:21:49 by pnita             #+#    #+#             */
/*   Updated: 2019/12/02 16:21:50 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

t_w_with_len	**change_ways(t_lem *lem, t_w_with_len **ways,
		int n_ways, t_w *new_way)
{
	t_w_with_len	**change_ways;
	int				i;
	t_l				*tmp_wl;

	i = 0;
	while (i < n_ways)
		paste_way_link_at_r(ways[i++]->way);
	paste_way_link_at_r(new_way);
	change_ways = create_ways(n_ways + 1);
	i = 0;
	tmp_wl = lem->start->way_links;
	while (i < n_ways + 1)
	{
		change_ways[i]->way = create_way(lem->start);
		add_way(change_ways[i++]->way, find_r(lem, tmp_wl->name));
		tmp_wl = tmp_wl->next;
	}
	i = 0;
	while (i < n_ways + 1)
		paste_way(lem, change_ways[i++]->way);
	clear_after_change_ways(lem, ways, n_ways, new_way);
	return (change_ways);
}

void			set_ways_len_and_sort(t_w_with_len **ways, int n_ways)
{
	int				i;
	t_w_with_len	*tmp_w;

	i = 0;
	while (i < n_ways)
	{
		ways[i]->len = len_of_way(ways[i]->way);
		++i;
	}
	i = 0;
	while (i + 1 < n_ways)
	{
		if (ways[i]->len > ways[i + 1]->len)
		{
			tmp_w = ways[i];
			ways[i] = ways[i + 1];
			ways[i + 1] = tmp_w;
			i = 0;
		}
		++i;
	}
}

void			delete_way(t_w **way)
{
	t_w	*tmp_way;
	t_w	*ptr_way;

	tmp_way = *way;
	while (tmp_way)
	{
		ptr_way = tmp_way->next;
		free(tmp_way);
		tmp_way = ptr_way;
	}
	*way = NULL;
}

void			delete_ways(t_w_with_len ***ways, int n_ways)
{
	int	i;

	i = 0;
	while (i < n_ways)
	{
		delete_way(&(*ways)[i]->way);
		free((*ways)[i]);
		(*ways)[i] = NULL;
		++i;
	}
	free(*ways);
	*ways = NULL;
}
