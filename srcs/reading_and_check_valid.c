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

void		give_error(t_lem *lem, char **line)
{
	ft_strdel(line);
	delete_default(lem);
	write(2, "ERROR\n", 6);
	exit(1);
}

void		read_and_valid_ants(int fd, t_lem *lem)
{
	char	*line;
	
	while (get_next_line(fd, &line) > 0)
	{
		join_to_g_input_str(line);
		if (!ft_strncmp(line, "##start", 7) || !ft_strncmp(line, "##end", 5))
			give_error(lem, &line);
		if (line[0] != '#')
		{
			if (!ft_strn_only_digit(line, -1) || (lem->ants_num = ft_atoi(line)) <= 0) // check that only number, check that > 0
				give_error(lem, &line);
			ft_strdel(&line);
			return ;
		}
		else
			ft_strdel(&line);
	}
}

size_t	hash_koord(int x, int y)
{
	size_t	hash;

	hash = x * 5 + y * 3;
	return (hash % ROOMS_SIZE);
}

t_koord	*create_koord(int x, int y)
{
	t_koord	*koord;

	koord = (t_koord*)ft_memalloc(sizeof(t_koord));
	koord->x = x;
	koord->y = y;
	return (koord);
}

int		check_koord_only_digit(t_lem *lem, char *str)
{
	char	*ptr;
	int		x;
	int		y;
	t_koord	*same_koord;

	ptr = str;
	while (*ptr && *ptr != ' ')
		++ptr;
	if (!*ptr || !ft_strn_only_digit(str, ptr - str) || !ft_strn_only_digit(ptr + 1, -1) || (x = ft_atoi(str)) < 0 || (y = ft_atoi(ptr + 1)) < 0)
		return (0);
	lem->h_i = hash_koord(x, y);
	if (!lem->koord[lem->h_i])
		lem->koord[lem->h_i] = create_koord(x, y);
	else
	{
		same_koord = lem->koord[lem->h_i];
		while (same_koord->same_koord)
		{
			if (same_koord->x == x && same_koord->y == y)
				return (0);
			same_koord = same_koord->same_koord;
		}
		if (same_koord->x == x && same_koord->y == y)
			return (0);
		same_koord->same_koord = create_koord(x, y);
	}
	return (1);
}

void		reading_and_check_valid(int fd, t_lem *lem)
{
	char	*line;
	char	*name;
	char	*name2;
	int		i;
	t_r		*room1;
	t_r		*room2;
	int		flag;

	line = NULL;
	name = NULL;
	name2 = NULL;
	flag = 0;
	read_and_valid_ants(fd, lem);
	while (get_next_line(fd, &line) > 0)
	{
		join_to_g_input_str(line);
		if (!ft_strcmp(line, "##start"))
		{
			if (lem->start || lem->is_start)
				give_error(lem, &line);
			lem->is_start = 1;
		}
		else if (!ft_strcmp(line, "##end"))
		{
			if (lem->end || lem->is_end)
				give_error(lem, &line);
			lem->is_end = 1;
		}
		else if (ft_strncmp(line, "#", 1))
		{
			i = 0;
			while (line[i] && line[i] != ' ' && line[i] != '-')
				++i;
			if (!line[i]) // end of line
				give_error(lem, &line);
			else
			{
				name = ft_strsub(line, 0, i);
				if (line[i] == ' ')
				{
					if (flag == 2 || name[0] == 'L' || !check_koord_only_digit(lem, line + i + 1)) // check koordinates
					{
						ft_strdel(&name);
						give_error(lem, &line);
					}
					flag = 1;
					lem->h_i = hash(name);
					if (!lem->r[lem->h_i])
						lem->r[lem->h_i] = create_room(name, lem);
					else
					{
						if (find_r(lem, name)) // check that hasn't the same name
						{
							ft_strdel(&name);
							give_error(lem, &line);
						}
						add_same_num_room(lem->r[lem->h_i], name, lem);
					}
					lem->is_start = 0;
					lem->is_end = 0;
				}
				else if (line[i] == '-')
				{
					if (flag == 0)
					{
						ft_strdel(&name);
						give_error(lem, &line);
					}
					flag = 2;
					name2 = ft_strdup(line + i + 1);
					// check that is'n link to the same room,  rooms with theese names exist, check that link in one to another doesn't exist and in another to one
					if (!ft_strcmp(name, name2) || !(room1 = find_r(lem, name)) || !(room2 = find_r(lem, name2)) || find_link(room1, name2) || find_link(room2, name))
					{
						ft_strdel(&name);
						ft_strdel(&name2);
						give_error(lem, &line);
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
	if (!lem->start || !lem->end || flag != 2)
		give_error(lem, NULL);
}
