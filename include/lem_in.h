/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 19:17:28 by pnita             #+#    #+#             */
/*   Updated: 2019/10/20 19:17:31 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include "../ft_printf/include/ft_printf.h"

# define ROOMS_SIZE 10000
# define G_INPUT_STR_SIZE 100000

typedef struct		s_links
{
	char			*name;
	int				not_active;
	struct s_links	*next;
}					t_l;

typedef struct		s_koord
{
	int				x;
	int				y;
	struct s_koord	*same_koord;
}					t_koord;

typedef struct		s_room
{
	char			*name;
	struct s_room	*same_num;
	t_l				*links;
	struct s_room	*room_in;
	struct s_room	*room_out;
	int				moved_inside;
	struct s_room	*next_in_q;
	struct s_room	*prev_in_q;
	int				was_in_q;
	int				steps_in_q;
	t_l				*way_links;
}					t_r;

typedef struct		s_lem
{
	int				i;
	t_r				**r;
	int				ants_num;
	size_t			h_i;
	int				is_start;
	int				is_end;
	t_r				*start;
	t_r				*end;
	int				meet_end;
	t_koord			**koord;
}					t_lem;

typedef struct		s_way
{
	t_r				*r;
	struct s_way	*next;
}					t_w;

typedef struct		s_way_with_len
{
	struct s_way	*way;
	int				len;
	int				k;
}					t_w_with_len;

typedef struct		s_output
{
	int				ant_num;
	t_w				*way_point;
	struct s_output	*next;
}					t_output;

char				*g_input_str;
int					g_input_size;

t_lem				*create_lem(void);
void				delete_default(t_lem *lem);
size_t				hash(char *input);
t_r					*create_room(char *name, t_lem *lem);
void				add_same_num_room(t_r *r, char *name, t_lem *lem);
t_l					*create_link(char *name);
t_l					*find_link(t_r *room, char *name);
void				add_link(t_r *r, char *name);
void				add_way_link(t_r *r, char *name);
void				delete_way_link(t_r *room);
t_r					*find_r(t_lem *p, char *name);
void				join_to_g_input_str(char *s);
void				give_error(t_lem *lem, char **line);
int					check_koord_only_digit(t_lem *lem, char *str);
void				read_leaks(t_lem *lem, char **line, int *flag, char *name);
void				read_rooms(t_lem *lem, char **line, int *flag, char *name);
void				read_rooms_and_leaks(t_lem *lem,
		char **line_origin, int *flag);
void				read_start(t_lem *lem, char **line);
void				read_end(t_lem *lem, char **line);
void				reading_and_check_valid(int fd, t_lem *lem);
void				add_r_in_queque(t_r *queque, t_r *r_to_add);
t_w					*create_way(t_r *r);
void				push_to_begin_w(t_w **way, t_r *r);
void				add_way(t_w *way, t_r *r);
t_w					*get_way(t_lem *p);
t_w					*bfs(t_lem *lem);
void				clear_after_bfs(t_lem *lem);
void				reverse_way(t_w *way, t_lem *lem);
int					len_of_way(t_w *way);
int					tmp_not_in_next(t_r *tmp, t_r *next);
t_w_with_len		**create_ways(int n);
void				paste_way_link_at_r(t_w *way);
void				paste_way(t_lem *lem, t_w *way);
void				clear_after_change_ways(t_lem *lem, t_w_with_len **ways,
		int n_ways, t_w *new_way);
t_output			*create_output(int ant_num, t_w *way_point);
void				add_output(t_output **output, int ant_num, t_w *way_point);
void				print_output2(t_lem *lem, t_output **output);
void				print_output(t_lem *lem, t_output **output);
void				update_output(t_output *output);
t_w_with_len		**change_ways(t_lem *lem, t_w_with_len **ways,
		int n_ways, t_w *new_way);
void				set_ways_len_and_sort(t_w_with_len **ways, int n_ways);
void				delete_way(t_w **way);
void				delete_ways(t_w_with_len ***ways, int n_ways);
void				delete_links(t_l **links);
void				delete_room(t_r **room);
void				delete_rooms(t_lem *lem);
void				delete_koord(t_lem *lem);

#endif
