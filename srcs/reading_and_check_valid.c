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
