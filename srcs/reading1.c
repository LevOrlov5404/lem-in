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

#include "lem_in.h"

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

void	give_error(t_lem *lem, char **line)
{
	ft_strdel(line);
	delete_default(lem);
	write(2, "ERROR\n", 6);
	exit(1);
}

void	read_and_valid_ants(int fd, t_lem *lem)
{
	char	*line;

	while (get_next_line(fd, &line) > 0)
	{
		join_to_g_input_str(line);
		if (!ft_strncmp(line, "##start", 7) || !ft_strncmp(line, "##end", 5))
			give_error(lem, &line);
		if (line[0] != '#')
		{
			if (!ft_strn_only_digit(line, -1)
					|| (lem->ants_num = ft_atoi(line)) <= 0
					|| lem->ants_num > 20000)
				give_error(lem, &line);
			ft_strdel(&line);
			return ;
		}
		else
			ft_strdel(&line);
	}
}

void	reading_and_check_valid(int fd, t_lem *lem)
{
	char	*line;
	int		flag;

	line = NULL;
	flag = 0;
	read_and_valid_ants(fd, lem);
	while (get_next_line(fd, &line) > 0)
	{
		join_to_g_input_str(line);
		if (!ft_strcmp(line, "##start"))
			read_start(lem, &line);
		else if (!ft_strcmp(line, "##end"))
			read_end(lem, &line);
		else if (!ft_strncmp(line, "#", 1))
		{
			if (lem->is_start || lem->is_end)
				give_error(lem, &line);
		}
		else
			read_rooms_and_leaks(lem, &line, &flag);
		ft_strdel(&line);
	}
	if (!lem->start || !lem->end || flag != 2)
		give_error(lem, NULL);
}
