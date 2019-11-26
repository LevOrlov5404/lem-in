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

void	print_way(t_w *way, t_lem *lem)
{
	t_r	*tmp_r;

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

t_w		**change_ways_first(int n_ways, t_w **way, t_lem *lem)
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
	tmp_wl = lem->start->way_links;
	while (i < n_ways)
	{
		new_way[i++] = create_way(find_r(lem, tmp_wl->name));
		tmp_wl = tmp_wl->next;
	}
	i = 0;
	while (i < n_ways)
	{
		tmp_w = new_way[i++];
		tmp_r = tmp_w->r;
		while (tmp_r != lem->end)
		{
			tmp_wl = tmp_r->way_links;
			while (tmp_wl && tmp_wl->not_active)
				tmp_wl = tmp_wl->next;
			next_r = find_r(lem, tmp_wl->name);
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

t_w_with_len		**create_ways(int n)
{
	t_w_with_len	**ways;
	int				i;

	ways = (t_w_with_len**)ft_memalloc(sizeof(t_w_with_len*) * n);
	i = 0;
	while (i < n)
		ways[i++] = (t_w_with_len*)ft_memalloc(sizeof(t_w_with_len));
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

void				paste_way(t_lem *lem, t_w *way)
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
		if (is_tmp_in_next(tmp_r, next_r))
		{
			add_way(way, next_r);
			tmp_r = next_r;
		}
		else
			tmp_wl->not_active = 1;
	}
}

void				clear_after_change_ways(t_lem *lem, t_w_with_len **ways, int n_ways, t_w *new_way)
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
}

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
		// printf("%d\n", ways[i]->k);
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

t_output	*create_output(int ant_num, t_w *way_point)
{
	t_output	*output;

	output = (t_output*)ft_memalloc(sizeof(t_output));
	output->ant_num = ant_num;
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

void		print_output(t_lem *lem, t_output **output)
{
	t_output	*tmp_output;
	t_output	*next_output;
	t_output	*ptr_output;

	while (*output)
	{
		if ((*output)->way_point->r != lem->end)
			break ;
		tmp_output = *output;
		printf("L%d-%s ", tmp_output->ant_num, tmp_output->way_point->r->name);
		*output = (*output)->next;
		free(tmp_output);
	}
	if (*output)
	{
		tmp_output = *output;
		while (tmp_output)
		{
			printf("L%d-%s ", tmp_output->ant_num, tmp_output->way_point->r->name);
			tmp_output = tmp_output->next;
		}
		tmp_output = *output;
		while (tmp_output->next)
		{
			next_output = tmp_output->next;
			if (next_output->way_point->r == lem->end)
			{
				if (next_output->way_point->r == lem->end)
				{
					do
					{
						ptr_output = next_output;
						next_output = next_output->next;
						free(ptr_output);
					} while (next_output->way_point->r == lem->end);
					tmp_output->next = next_output;
				}
			}
			tmp_output = tmp_output->next;
		}
	}
	printf("\n");
}

void		update_output(t_lem *lem, t_output *output)
{
	t_output	*tmp_output;
	t_output	*next_output;

	tmp_output = output;
	while (tmp_output)
	{
		tmp_output->way_point = tmp_output->way_point->next;
		tmp_output = tmp_output->next;
	}
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
	while (i < n_ways)
	{
		if (ants > ways[i]->k)
		{
			--ants;
			add_output(&output, ant_num++, ways[i]->way->next);
		}
		++i;
	}
	print_output(lem, &output);
	update_output(lem, output);
	while (output)
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
		update_output(lem, output);
	}
	// printf("\ncount_str = %d\n", count_str);
}

void		solve(t_lem *lem)
{
	t_w_with_len	**ways;
	t_w_with_len	**next_ways;
	t_w		*new_way;
	int		n_ways;

	// printf("in solve\n");
	new_way = bfs(lem);
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
		// delete old ways and new_way
		ways = next_ways;
		if (!check_usefull(lem, ways, n_ways))
			break ;
	}
	g_input_str[g_input_size++] = '\n';
	g_input_str[g_input_size] = '\0';
	write(1, g_input_str, g_input_size);
	shape_output(lem, ways, n_ways);
}

void		delete_links(t_l **links)
{
	t_l	*link;
	t_l	*ptr_link;

	link = *links;
	while (link)
	{
		ptr_link = link->next;
		ft_strdel(&link->name);
		free(link);
		link = ptr_link;
	}
	*links = NULL;
}

void		delete_room(t_r **room)
{
	ft_strdel(&(*room)->name);
	delete_links(&(*room)->links);
	delete_links(&(*room)->way_links);
	*room = NULL;
}

void		delete_rooms(t_lem *lem)
{
	int	i;
	t_r	*room;
	t_r *ptr_room;

	i = 0;
	while (i < ROOMS_SIZE)
	{
		if (lem->r[i])
		{
			room = lem->r[i];
			while (room->same_num)
			{
				ptr_room = room->same_num;
				if (room->room_out)
					delete_room(&room->room_out);
				delete_room(&room);
				room = ptr_room;
			}
			if (room->room_out)
				delete_room(&room->room_out);
			delete_room(&room);
		}
		++i;
	}
	free(lem->r);
	lem->r = NULL;
}

int			main(void)
{
	t_lem		*lem;
	int	fd;

	lem = create_lem();
	// fd = open("/home/lev/mywork/lem-in/big-superposition", O_RDONLY);
	fd = 0;
	g_input_str = (char*)ft_memalloc(sizeof(char) * (G_INPUT_STR_SIZE + 1));
	g_input_size = 0;
	g_input_str[g_input_size] = '\0';
	reading_and_check_valid(fd, lem);
	solve(lem);
	// delete
	delete_rooms(lem);
	free(lem);
	lem = NULL;
	return (0);
}
