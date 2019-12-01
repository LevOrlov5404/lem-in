/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 23:02:35 by pnita             #+#    #+#             */
/*   Updated: 2019/12/01 23:02:37 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

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
		*output = create_output(ant_num, way_point);
	else
	{
		tmp_output = *output;
		while (tmp_output->next)
			tmp_output = tmp_output->next;
		tmp_output->next = create_output(ant_num, way_point);
	}
}

void		print_output2(t_lem *lem, t_output **output)
{
	t_output	*tmp_output;
	t_output	*next_output;
	t_output	*ptr_output;

	tmp_output = *output;
	while (tmp_output->next)
	{
		next_output = tmp_output->next;
		if (next_output->way_point->r == lem->end)
		{
			while (next_output->way_point->r == lem->end)
			{
				ptr_output = next_output;
				next_output = next_output->next;
				free(ptr_output);
			}
			tmp_output->next = next_output;
		}
		tmp_output = tmp_output->next;
	}
}

void		print_output(t_lem *lem, t_output **output)
{
	t_output	*tmp;

	while (*output)
	{
		if ((*output)->way_point->r != lem->end)
			break ;
		tmp = *output;
		ft_printf("L%d-%s ", tmp->ant_num, tmp->way_point->r->name);
		*output = (*output)->next;
		free(tmp);
	}
	if (*output)
	{
		tmp = *output;
		while (tmp)
		{
			ft_printf("L%d-%s ", tmp->ant_num, tmp->way_point->r->name);
			tmp = tmp->next;
		}
		print_output2(lem, output);
	}
	ft_printf("\n");
}

void		update_output(t_output *output)
{
	t_output	*tmp_output;

	tmp_output = output;
	while (tmp_output)
	{
		tmp_output->way_point = tmp_output->way_point->next;
		tmp_output = tmp_output->next;
	}
}
