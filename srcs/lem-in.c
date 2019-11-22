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

void	print_way(t_w *way, t_p *par)
{
	t_r	*tmp_r;
	
	printf("%d steps way: ", par->end->steps_in_q);
	while (way)
	{
		printf("%s->", way->r->name);
		way = way->next;
	}
	printf("NULL\n");
}

int		is_tmp_in_next(t_r *tmp, t_r *next)
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

t_w		**change_ways_first(int n_ways, t_w **way, t_p *par)
{
	t_w **new_way;
	t_w	*tmp_w;
	t_r	*tmp_r;
	t_r *next_r;
	t_l	*tmp_wl;
	int	i;

	i = 0;
	while (i < n_ways)
	{
		tmp_w = way[i++];
		while (tmp_w->next)
		{
			tmp_r = tmp_w->r;
			next_r = tmp_w->next->r;
			add_way_link(tmp_r->room_in ? tmp_r->room_in : tmp_r, next_r->name);
			tmp_w = tmp_w->next;
		}
	}
	new_way = (t_w**)ft_memalloc(sizeof(t_w) * n_ways);
	i = 0;
	tmp_wl = par->start->way_links;
	while (i < n_ways)
	{
		new_way[i++] = create_way(find_r(par, tmp_wl->name));
		tmp_wl = tmp_wl->next;
	}
	i = 0;
	while (i < n_ways)
	{
		tmp_w = new_way[i++];
		tmp_r = tmp_w->r;
		while (tmp_r != par->end)
		{
			tmp_wl = tmp_r->way_links;
			while (tmp_wl && tmp_wl->not_active)
				tmp_wl = tmp_wl->next;
			next_r = find_r(par, tmp_wl->name);
			if (is_tmp_in_next(tmp_r, next_r))
			{
				add_way(tmp_w, next_r);
				tmp_r = next_r;
			}
			else
				tmp_wl->not_active = 1;
		}
	}
	return (new_way);
}

t_w_with_len		**create_ways(n)
{
	t_w_with_len	**ways;
	int				i;

	ways = ft_memalloc(sizeof(t_w_with_len**) * n);
	i = 0;
	while (i < n)
		ways[i++] = ft_memalloc(sizeof(t_w_with_len*));
	return (ways);
}

void				paste_way_link_at_r(t_w *way)
{
	t_r	*tmp_r;
	t_r	*next_r;

	while (way->next)
	{
		tmp_r = way->r;
		next_r = way->next->r;
		add_way_link(tmp_r->room_in ? tmp_r->room_in : tmp_r, next_r->name);
		way = way->next;
	}
}

void				paste_way(t_p *par, t_w *way)
{
	t_r	*tmp_r;
	t_r *next_r;
	t_l	*tmp_wl;

	tmp_r = way->next->r;
	while (tmp_r != par->end)
	{
		tmp_wl = tmp_r->way_links;
		while (tmp_wl && tmp_wl->not_active)
			tmp_wl = tmp_wl->next;
		next_r = find_r(par, tmp_wl->name);
		if (is_tmp_in_next(tmp_r, next_r))
		{
			add_way(way, next_r);
			tmp_r = next_r;
		}
		else
			tmp_wl->not_active = 1;
	}
}

void				clear_after_change_ways(t_p *par, t_w_with_len **ways, int n_ways, t_w *new_way)
{
	int	i;
	t_w	*tmp_w;

	i = 0;
	while (i < n_ways)
	{
		tmp_w = ways[i++]->way;
		while (tmp_w->r != par->end)
		{
			delete_way_link(tmp_w->r);
			tmp_w = tmp_w->next;
		}
	}
}

t_w_with_len		**change_ways(t_p *par, t_w_with_len **ways, int n_ways, t_w *new_way)
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
	tmp_wl = par->start->way_links;
	while (i < n_ways + 1)
	{
		change_ways[i]->way = create_way(par->start);
		add_way(change_ways[i++]->way, find_r(par, tmp_wl->name));
		tmp_wl = tmp_wl->next;
	}
	i = 0;
	while (i < n_ways + 1)
		paste_way(par, change_ways[i++]->way);
	clear_after_change_ways(par, ways, n_ways, new_way);
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
		// printf("%d\n", ways[i]->len);
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

int		check_usefull(t_p *par, t_w_with_len **ways, int n_ways)
{
	int	i;
	int	len_sum;
	int	ants;

	i = 0;
	len_sum = 0;
	while (i < n_ways)
	{
		ways[i]->k = i * ways[i]->len - len_sum;
		// printf("%d\n", ways[i]->k);
		len_sum += ways[i]->len;
		++i;
	}
	ants = par->ants_num;
	i = 0;
	while (i < n_ways)
	{
		if (!(ants-- > ways[i++]->k))
			return (0);
	}
	return (1);
}

t_output	*create_output(int ant_num, t_w *way_point)
{
	t_output	*output;

	output = ft_memalloc(sizeof(t_output*));
	output->ant_num = ant_num;
	printf("in create ant_num = %d\n", output->ant_num);
	output->way_point = way_point;
	output->next = NULL;
	return (output);
}

void		add_output(t_output **output, int ant_num, t_w *way_point)
{
	t_output	*tmp_output;
	
	if (!*output)
	{
		*output = create_output(ant_num, way_point);
		// printf("in add ant_num = %d\n", (*output)->ant_num);
	}
	else
	{
		tmp_output = *output;
		while (tmp_output->next)
			tmp_output = tmp_output->next;
		tmp_output->next = create_output(ant_num, way_point);
		// printf("in add ant_num = %d\n", tmp_output->next->ant_num);
	}
}

void		print_output(t_output *output)
{
	t_output	*tmp_output;

	tmp_output = output;
	while (tmp_output)
	{
		// printf("in print ant_num = %d\n", tmp_output->ant_num);
		printf("L%d-%s ", tmp_output->ant_num, tmp_output->way_point->r->name);
		tmp_output = tmp_output->next;
	}
	printf("\n");
}

t_output	*swipe_output_while_end_point(t_p * par, t_output *output)
{
	t_w			*way_point;
	t_output	*next_output;
	
	next_output = output;
	way_point = next_output->way_point;
	while (next_output && way_point->r == par->end)
	{
		next_output = next_output->next;
		way_point = next_output->way_point;
	}
	return (next_output);
}

void		swipe_output_way_point(t_output *output)
{
	while (output)
	{
		output->way_point = output->way_point->next;
		output = output->next;
	}
}

void		shape_output(t_p *par, t_w_with_len **ways, int n_ways)
{
	int			ants;
	int			ant_num;
	int			i;
	t_output	*tmp_output;
	t_output	*next_output;

	ants = par->ants_num;
	ant_num = 1;
	tmp_output = NULL;
	i = 0;
	while (i < n_ways)
	{
		if (ants-- > ways[i]->k)
		{
			// printf("____ counter ant_num = %d\n", ant_num);
			add_output(&tmp_output, ant_num++, ways[i++]->way->next);
		}
	}
	print_output(tmp_output);
	next_output = swipe_output_while_end_point(par, tmp_output);
	swipe_output_way_point(next_output);
	i = 0;
	while (i < n_ways)
	{
		if (ants-- > ways[i]->k)
		{
			// printf("____ counter ant_num = %d\n", ant_num);
			add_output(&next_output, ant_num++, ways[i++]->way->next);
		}
	}
	print_output(next_output);
	// while (ants)
	// {
	// }
}

void		solve(t_p *par)
{
	t_w_with_len	**ways;
	t_w_with_len	**next_ways;
	t_w		*new_way;
	int		n_ways;

	new_way = bfs(par);
	reverse_way(new_way, par);
	clear_after_bfs(par);
	n_ways = 1;
	ways = create_ways(n_ways);
	ways[0]->way = new_way;
	ways[0]->len = par->end->steps_in_q;
	while ((new_way = bfs(par)))
	{
		reverse_way(new_way, par);
		next_ways = change_ways(par, ways, n_ways++, new_way);
		set_ways_len_and_sort(next_ways, n_ways);
		ways = next_ways;
		if (!check_usefull(par, ways, n_ways))
			break ;
	}
	// output by sent_ant_with_check
	shape_output(par, ways, n_ways);
}

int			main(void)
{
	t_p		*par;
	int	fd;

	par = create_par();
	fd = open("/Users/pnita/my_work/lem-in/kek4", O_RDWR);
	reading_and_check_valid(fd, par);
	// printf("start room = %s\n", par->start->name);
	// printf("end room = %s\n", par->end->name);
	// printf("rooms_num_read = %d rooms_num_fact = %d\n", par->rooms_num_read, par->rooms_num_fact);
	solve(par);
	// delete
	free(par->r);
	par->r = NULL;
	free(par);
	par = NULL;
	return (0);
}
