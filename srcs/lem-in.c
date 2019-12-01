/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem-in.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 19:17:21 by pnita             #+#    #+#             */
/*   Updated: 2019/10/20 19:17:23 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

t_w_with_len		**change_ways(t_lem *lem, t_w_with_len **ways, int n_ways, t_w *new_way)
{
	t_w_with_len	**change_ways;
	int				i;
	t_l	*tmp_wl;

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

void		set_ways_len_and_sort(t_w_with_len **ways, int n_ways)
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

int		check_usefull(t_lem *lem, t_w_with_len **ways, int n_ways)
{
	int	i;
	int	len_sum;
	int	ants;

	i = 0;
	len_sum = 0;
	while (i < n_ways)
	{
		ways[i]->k = i * ways[i]->len - len_sum;
		len_sum += ways[i]->len;
		++i;
	}
	ants = lem->ants_num;
	i = 0;
	while (i < n_ways)
	{
		if (!(ants-- > ways[i++]->k))
			return (0);
	}
	return (1);
}

void		delete_way(t_w **way)
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

void		delete_ways(t_w_with_len ***ways, int n_ways)
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

void		shape_output(t_lem *lem, t_w_with_len **ways, int n_ways)
{
	int			ants;
	int			ant_num;
	int			i;
	t_output	*output;

	ants = lem->ants_num;
	ant_num = 1;
	output = NULL;
	i = 0;
	while (ants || output)
	{
		i = 0;
		while (ants && i < n_ways)
		{
			if (ants > ways[i]->k)
			{
				--ants;
				add_output(&output, ant_num++, ways[i]->way->next);
			}
			++i;
		}
		print_output(lem, &output);
		update_output(output);
	}
	delete_ways(&ways, n_ways);
}

void		solve(t_lem *lem)
{
	t_w_with_len	**ways;
	t_w_with_len	**next_ways;
	t_w				*new_way;
	int				n_ways;

	if (!(new_way = bfs(lem)))
		give_error(lem, NULL);
	reverse_way(new_way, lem);
	n_ways = 1;
	ways = create_ways(n_ways);
	ways[0]->way = new_way;
	ways[0]->len = lem->end->steps_in_q;
	while ((new_way = bfs(lem)))
	{
		reverse_way(new_way, lem);
		next_ways = change_ways(lem, ways, n_ways++, new_way);
		set_ways_len_and_sort(next_ways, n_ways);
		delete_way(&new_way);
		delete_ways(&ways, n_ways - 1);
		ways = next_ways;
		if (!check_usefull(lem, ways, n_ways))
			break ;
	}
	delete_way(&new_way);
	g_input_str[g_input_size++] = '\n';
	g_input_str[g_input_size] = '\0';
	write(1, g_input_str, g_input_size);
	shape_output(lem, ways, n_ways);
}

int			main(void)
{
	t_lem		*lem;
	int	fd;

	lem = create_lem();
	// fd = open("/Users/pnita/my_work/git_lem-in/maps/invalid", O_RDONLY);
	// fd = open("/Users/pnita/my_work/git_lem-in/maps/big-superposition", O_RDONLY);
	fd = 0;
	g_input_str = (char*)ft_memalloc(sizeof(char) * (G_INPUT_STR_SIZE + 1));
	g_input_size = 0;
	g_input_str[g_input_size] = '\0';
	reading_and_check_valid(fd, lem);
	solve(lem);
	delete_default(lem);
	return (0);
}
