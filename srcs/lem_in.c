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

#include "lem_in.h"

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

void	shape_output(t_lem *lem, t_w_with_len **ways, int n_ways)
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

int		step_in_solve(t_lem *lem, t_w_with_len ***ways,
		int *n_ways, t_w **new_way)
{
	t_w_with_len	**next_ways;

	reverse_way(*new_way, lem);
	next_ways = change_ways(lem, *ways, (*n_ways)++, *new_way);
	set_ways_len_and_sort(next_ways, *n_ways);
	delete_way(new_way);
	if (!check_usefull(lem, next_ways, *n_ways))
	{
		delete_ways(&next_ways, *n_ways);
		--(*n_ways);
		return (0);
	}
	delete_ways(ways, *n_ways - 1);
	*ways = next_ways;
	return (1);
}

void	solve(t_lem *lem)
{
	t_w_with_len	**ways;
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
		if (!step_in_solve(lem, &ways, &n_ways, &new_way))
			break ;
	}
	delete_way(&new_way);
	g_input_str[g_input_size++] = '\n';
	g_input_str[g_input_size] = '\0';
	write(1, g_input_str, g_input_size);
	shape_output(lem, ways, n_ways);
}

int		main(void)
{
	t_lem		*lem;
	int			fd;

	lem = create_lem();
	fd = 0;
	g_input_str = (char*)ft_memalloc(sizeof(char) * (G_INPUT_STR_SIZE + 1));
	g_input_size = 0;
	g_input_str[g_input_size] = '\0';
	reading_and_check_valid(fd, lem);
	solve(lem);
	delete_default(lem);
	return (0);
}
