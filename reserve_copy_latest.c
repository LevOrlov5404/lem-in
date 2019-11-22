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

t_l			*create_link(char *name)
{
	t_l	*link;

	link = (t_l*)ft_memalloc(sizeof(t_l));
	link->name = ft_strdup(name);
	return (link);
}

void		add_link(t_r *r, char *name)
{
	t_l	*tmp_link;
	
	if (!r->links)
		r->links = create_link(name);
	else
	{
		tmp_link = r->links;
		while (tmp_link->next)
			tmp_link = tmp_link->next;
		tmp_link->next = create_link(name);
	}
}

void		add_way_link(t_r *r, char *name)
{
	t_l	*tmp_link;
	
	if (!r->way_links)
		r->way_links = create_link(name);
	else
	{
		tmp_link = r->way_links;
		while (tmp_link->next)
			tmp_link = tmp_link->next;
		tmp_link->next = create_link(name);
	}
}

void		add_r_in_queque(t_r *queque, t_r *r_to_add)
{
	while (queque->next_in_q)
		queque = queque->next_in_q;
	queque->next_in_q = r_to_add;
	r_to_add->was_in_q = 1;
}

t_w		*create_way(t_r *r)
{
	t_w	*way;

	way = (t_w*)ft_memalloc(sizeof(t_w));
	way->r = r;
	return (way);
}

void	push_to_begin_w(t_w **way, t_r *r)
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

void	add_way(t_w *way, t_r *r)
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

t_w		*get_way(t_p *p)
{
	t_r	*queque;
	t_r	*tmp_r;
	t_l	*tmp_link;
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
	return (way);
}

t_w		*bfs(t_p *p)
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
		if (queque->room_in && !queque->room_in->steps_in_q)
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

void	clear_after_bfs(t_p *par)
{
	t_r	*r;
	t_r	*next_r;

	r = par->start;
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
	par->meet_end = 0;
}

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

void		reverse_way(t_w *way, t_p *par)
{
	t_w	*w;
	t_w	*next_w;
	t_l	*tmp_link;
	char *r_name_before;
	t_r	*room_out;

	w = way;
	next_w = w->next;
	tmp_link = w->r->links;
	while (ft_strcmp(tmp_link->name, next_w->r->name))
		tmp_link = tmp_link->next;
	tmp_link->not_active = 1;
	r_name_before = w->r->name;
	while (next_w->r != par->end)
	{
		w = next_w;
		next_w = next_w->next;
		tmp_link = w->r->links;
		if (!w->r->room_out)
		{
			while (ft_strcmp(tmp_link->name, next_w->r->name))
				tmp_link = tmp_link->next;
		}
		tmp_link->not_active = 1;
		room_out = create_room(w->r->name, par);
		room_out->links = w->r->links;
		room_out->room_in = w->r;
		w->r->room_out = room_out;
		w->r->links = create_link(r_name_before);
		r_name_before = w->r->name;
	}
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

t_w		**change_ways(int n_ways, t_w **way, t_p *par)
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

int			main(void)
{
	t_p		*par;
	int	fd;
	t_w	**way;
	t_w	**changed_way;
	int	i;
	// t_sol *sol;

	par = create_par();
	fd = open("/Users/pnita/my_work/lem-in/kek4", O_RDWR);
	reading_and_check_valid(fd, par);
	printf("start room = %s\n", par->start->name);
	printf("end room = %s\n", par->end->name);
	// printf("rooms_num_read = %d rooms_num_fact = %d\n", par->rooms_num_read, par->rooms_num_fact);
	way = (t_w**)ft_memalloc(sizeof(t_w*) * 100);
	i = 0;
	while (1)
	{
		if (!(way[i] = bfs(par)))
			break ;
		print_way(way[i], par);
		reverse_way(way[i], par);
		clear_after_bfs(par);
		++i;
	}
	printf("kolvo putey = %i\n", i);
	changed_way = change_ways(i, way, par);
	// delete
	free(par->r);
	par->r = NULL;
	free(par);
	par = NULL;
	return (0);
}
