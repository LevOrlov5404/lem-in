/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading_and_check_valid.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 14:02:51 by pnita             #+#    #+#             */
/*   Updated: 2019/11/10 14:02:54 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

void	join_to_g_input_str(char *s)
{
	if (*s)
	{
		while (*s)
			g_input_str[g_input_size++] = *(s++);
		g_input_str[g_input_size++] = '\n';
		g_input_str[g_input_size] = '\0';
	}
}

t_l			*find_link(t_r *room, char *name)
{
	t_l	*tmp_link;

	tmp_link = room->links;
	while (tmp_link && ft_strcmp(tmp_link->name, name))
		tmp_link = tmp_link->next;
	return (tmp_link);
}

void		reading_and_check_valid(int fd, t_lem *lem)
{
	char	*line;
	char	*name;
	char	*name2;
	int		i;
	t_r		*room1;
	t_r		*room2;
	int		got_ants;
	int		got_rooms_block;

	line = NULL;
	name = NULL;
	name2 = NULL;
	got_ants = 0;
	got_rooms_block = 0;
	// сначала нужно сделать пока не считаем ants (ведь это должно быть первой строкой), ##start ##end => error, остальные комменты игнорим 
	while (get_next_line(fd, &line) > 0)
	{
		join_to_g_input_str(line);
		if (!ft_strncmp(line, "##", 2))
		{
			if (!ft_strncmp(line, "##start", 7))
				lem->is_start = 1;
			else if (!ft_strncmp(line, "##end", 5))
				lem->is_end = 1;
		}
		else if (ft_strncmp(line, "#", 1))
		{
			i = 0;
			while (line[i] && line[i] != ' ' && line[i] != '-')
				++i;
			if (!line[i]) // number should be the first no comment string => fix it
			{
				if (!ft_str_only_digit(line) || lem->ants_num || (lem->ants_num = ft_atoi(line)) <= 0) // check that only number and only one time, check that > 0
				{
					ft_strdel(&line);
					delete_default(lem);
					write(2, "ERROR\n", 6);
					exit(1);
				}
			}
			else
			{
				name = ft_strsub(line, 0, i);
				if (line[i] == ' ')
				{
					lem->h_i = hash(name);
					if (!lem->r[lem->h_i])
						lem->r[lem->h_i] = create_room(name, lem);
					else
					{
						if (find_r(lem, name)) // check that hasn't the same name
						{
							ft_strdel(&name);
							ft_strdel(&line);
							delete_default(lem);
							write(2, "ERROR\n", 6);
							exit(1);
						}
						// check koordinates
						add_same_num_room(lem->r[lem->h_i], name, lem);
					}
					lem->is_start = 0;
					lem->is_end = 0;
				}
				else if (line[i] == '-')
				{
					name2 = ft_strdup(line + i + 1);
					// check that is'n link to the same room,  rooms with theese names exist, check that link in one to another doesn't exist and in another to one
					if (!ft_strcmp(name, name2) || !(room1 = find_r(lem, name)) || !(room2 = find_r(lem, name2)) || find_link(room1, name2) || find_link(room2, name))
					{
						ft_strdel(&name);
						ft_strdel(&name2);
						ft_strdel(&line);
						delete_default(lem);
						write(2, "ERROR\n", 6);
						exit(1);
					}
					add_link(room1, name2);
					add_link(room2, name);
					ft_strdel(&name2);
				}
				ft_strdel(&name);
			}
		}
		ft_strdel(&line);
	}
}
