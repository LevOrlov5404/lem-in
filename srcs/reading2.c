/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 17:50:35 by pnita             #+#    #+#             */
/*   Updated: 2019/12/02 17:50:36 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void	read_leaks(t_lem *lem, char **line, int *flag, char *name)
{
	t_r		*room1;
	t_r		*room2;
	char	*name2;

	room1 = NULL;
	room2 = NULL;
	if (*flag == 0)
	{
		ft_strdel(&name);
		give_error(lem, line);
	}
	*flag = 2;
	name2 = ft_strdup(*line + lem->i + 1);
	if (!ft_strcmp(name, name2) || !(room1 = find_r(lem, name))
			|| !(room2 = find_r(lem, name2))
			|| find_link(room1, name2) || find_link(room2, name))
	{
		ft_strdel(&name);
		ft_strdel(&name2);
		give_error(lem, line);
	}
	add_link(room1, name2);
	add_link(room2, name);
	ft_strdel(&name2);
}

void	read_rooms(t_lem *lem, char **line, int *flag, char *name)
{
	if (*flag == 2 || name[0] == 'L'
			|| !check_koord_only_digit(lem, *line + lem->i + 1))
	{
		ft_strdel(&name);
		give_error(lem, line);
	}
	*flag = 1;
	lem->h_i = hash(name);
	if (!lem->r[lem->h_i])
		lem->r[lem->h_i] = create_room(name, lem);
	else
	{
		if (find_r(lem, name))
		{
			ft_strdel(&name);
			give_error(lem, line);
		}
		add_same_num_room(lem->r[lem->h_i], name, lem);
	}
	lem->is_start = 0;
	lem->is_end = 0;
}

void	read_rooms_and_leaks(t_lem *lem, char **line_origin, int *flag)
{
	char	*line;
	char	*name;

	line = *line_origin;
	name = NULL;
	lem->i = 0;
	while (line[lem->i] && line[lem->i] != ' ' && line[lem->i] != '-')
		++lem->i;
	if (!lem->i || !line[lem->i])
		give_error(lem, line_origin);
	else
	{
		name = ft_strsub(line, 0, lem->i);
		if (line[lem->i] == ' ')
			read_rooms(lem, line_origin, flag, name);
		else if (line[lem->i] == '-')
			read_leaks(lem, line_origin, flag, name);
		ft_strdel(&name);
	}
}

void	read_start(t_lem *lem, char **line)
{
	if (lem->start || lem->is_start)
		give_error(lem, line);
	lem->is_start = 1;
}

void	read_end(t_lem *lem, char **line)
{
	if (lem->end || lem->is_end)
		give_error(lem, line);
	lem->is_end = 1;
}
