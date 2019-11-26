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

void		reading_and_check_valid(int fd, t_lem *lem)
{
	char	*line;
	char	*name;
	char	*name2;
	int		i;

	// printf("in reading\n");
	line = NULL;
	name = NULL;
	name2 = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		join_to_g_input_str(line);
		// check_valid
		if (!ft_strncmp(line, "##", 2))
		{
			if (!ft_strncmp(line, "##start", 7))
				lem->is_start = 1;
			else if (!ft_strncmp(line, "##end", 5))
				lem->is_end = 1;
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
				lem->ants_num = ft_atoi(line);
			else
			{
				name = ft_strsub(line, 0, i); // delete
				// printf("%s\n", name);
				if (line[i] == ' ')
				{
					// printf("____in create rooms\n");
					++lem->rooms_num;
					lem->h_i = hash(name);
					// printf("h_i of room %s = %zu\n", name, lem->h_i);
					if (!lem->r[lem->h_i])
						lem->r[lem->h_i] = create_room(name, lem);
					else
						add_same_num_room(lem->r[lem->h_i], name, lem);
					lem->is_start = 0;
					lem->is_end = 0;
				}
				else if (line[i] == '-')
				{
					// printf("____in create links____\n");
					name2 = ft_strdup(line + i + 1);
					add_link(find_r(lem, name), name2);
					// printf("%s link = %s\n", name, find_r(lem, name)->links->name);
					add_link(find_r(lem, name2), name);
					// printf("%s link = %s\n", name2, find_r(lem, name2)->links->name);
					ft_strdel(&name2);
				}
				ft_strdel(&name);
			}
		}
		ft_strdel(&line);
	}
}
