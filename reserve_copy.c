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

#define	R_SIZE 10000

typedef struct		s_links
{
	char			*name;
	int				not_active;
	// int				from_in;
	// int				weight;
	struct s_links	*next;
}					t_l;

typedef struct		s_room
{
	char			*name;
	struct s_room	*same_num;
	t_l				*links;
	// int				rev;
	struct s_room	*room_in;
	struct s_room	*room_out;
	struct s_room	*next_in_q;
	struct s_room	*prev_in_q;
	int				was_in_q;
	int				steps_in_q;
	t_l				*way_links;
}					t_r;

typedef struct		s_par
{
	t_r		**r;
	int		r_len;
	int		ants_num;
	int		rooms_num;
	size_t	h_i;
	int 	is_start;
	int 	is_end;
	t_r		*start;
	t_r		*end;
	int		meet_end;
}					t_p;

typedef struct		s_way
{
	t_r				*r;
	struct s_way	*next;
}					t_w;

typedef struct			s_solutions
{
	int					n_ways;
	struct s_way		**ways;
	struct s_solutions	*next;
}						t_solutions;

t_p			*create_par()
{
	t_p	*par;
	int	i;

	par = (t_p*)ft_memalloc(sizeof(t_p));
	par->r_len = R_SIZE;
	par->r = (t_r**)ft_memalloc(sizeof(t_r*) * par->r_len);
	return (par);
}

size_t		hash(char *input)
{
	size_t	hash_num;

	hash_num = 1;
	while (*input)
	{
		hash_num = hash_num * 3 + *(input++);
	}
	return hash_num % R_SIZE;
}

t_r			*create_room(char *name, t_p *par)
{
	t_r		*r;

	r = (t_r*)ft_memalloc(sizeof(t_r));
	r->name = ft_strdup(name);
	if (par->is_start)
		par->start = r;
	if (par->is_end)
		par->end = r;
	return (r);
}

void		add_same_num_room(t_r *r, char *name, t_p *par)
{
	t_r	*tmp_r;
	
	tmp_r = r;
	while (tmp_r->same_num)
		tmp_r = tmp_r->same_num;
	tmp_r->same_num = create_room(name, par);
}

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

t_r			*find_r(t_p *p, char *name)
{
	t_r		*tmp_r;

	tmp_r = p->r[hash(name)];
	while (tmp_r && ft_strcmp(tmp_r->name, name))
		tmp_r = tmp_r->same_num;
	return (tmp_r);
}

void		reading_and_check_valid(int fd, t_p *par)
{
	char	*line;
	char	*name;
	char	*name2;
	int		i;

	line = NULL;
	name = NULL;
	name2 = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		// check_valid
		if (!ft_strncmp(line, "##", 2))
		{
			if (!ft_strncmp(line, "##start", 7))
				par->is_start = 1;
			else if (!ft_strncmp(line, "##end", 5))
				par->is_end = 1;
		}
		else if (!ft_strncmp(line, "#", 1))
		{
			//comment - skip
		}
		else
		{
			i = 0;
			while (line[i] && line[i] != ' ' && line[i] != '-')
				++i;
			if (!line[i]) // check that only numbers
				par->ants_num = ft_atoi(line);
			else
			{
				name = ft_strsub(line, 0, i); // delete
				// printf("%s\n", name);
				if (line[i] == ' ')
				{
					// printf("____in create rooms\n");
					++par->rooms_num;
					par->h_i = hash(name);
					// printf("h_i of room %s = %zu\n", name, par->h_i);
					if (!par->r[par->h_i])
						par->r[par->h_i] = create_room(name, par);
					else
						add_same_num_room(par->r[par->h_i], name, par);
					par->is_start = 0;
					par->is_end = 0;
				}
				else if (line[i] == '-')
				{
					// printf("____in create links____\n");
					name2 = ft_strdup(line + i + 1);
					add_link(find_r(par, name), name2);
					// printf("%s link = %s\n", name, find_r(par, name)->links->name);
					add_link(find_r(par, name2), name);
					// printf("%s link = %s\n", name2, find_r(par, name2)->links->name);
				}
			}
		}
		ft_strdel(&line);
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

int			main(void)
{
	t_p		*par;
	int	fd;
	t_w	*way;

	par = create_par();
	fd = 0;//open("/Users/pnita/my_work/lem-in/kek4", O_RDWR);
	reading_and_check_valid(fd, par);
	printf("start room = %s\n", par->start->name);
	printf("end room = %s\n", par->end->name);
	// printf("rooms_num_read = %d rooms_num_fact = %d\n", par->rooms_num_read, par->rooms_num_fact);
	while (1)
	{
		if (!(way = bfs(par)))
			break ;
		print_way(way, par);
		reverse_way(way, par);
		clear_after_bfs(par);
	}
	// delete
	free(par->r);
	par->r = NULL;
	free(par);
	par = NULL;
	return (0);
}
