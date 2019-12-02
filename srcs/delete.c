/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 22:30:28 by pnita             #+#    #+#             */
/*   Updated: 2019/12/01 22:30:29 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void		delete_links(t_l **links)
{
	t_l	*link;
	t_l	*ptr_link;

	link = *links;
	while (link)
	{
		ptr_link = link->next;
		ft_strdel(&link->name);
		free(link);
		link = ptr_link;
	}
	*links = NULL;
}

void		delete_room(t_r **room)
{
	if ((*room)->name)
		ft_strdel(&(*room)->name);
	if ((*room)->links)
		delete_links(&(*room)->links);
	if ((*room)->way_links)
		delete_links(&(*room)->way_links);
	free(*room);
	*room = NULL;
}

void		delete_rooms(t_lem *lem)
{
	int	i;
	t_r	*room;
	t_r *ptr_room;

	i = 0;
	while (i < ROOMS_SIZE)
	{
		room = lem->r[i++];
		if (room)
		{
			while (room->same_num)
			{
				ptr_room = room->same_num;
				if (room->room_out)
					delete_room(&room->room_out);
				delete_room(&room);
				room = ptr_room;
			}
			if (room->room_out)
				delete_room(&room->room_out);
			delete_room(&room);
		}
	}
	free(lem->r);
	lem->r = NULL;
}

void		delete_koord(t_lem *lem)
{
	t_koord	*tmp_koord;
	t_koord	*ptr_koord;
	int		i;

	i = 0;
	while (i < ROOMS_SIZE)
	{
		tmp_koord = lem->koord[i++];
		if (tmp_koord)
		{
			while (tmp_koord->same_koord)
			{
				ptr_koord = tmp_koord->same_koord;
				free(tmp_koord);
				tmp_koord = ptr_koord;
			}
			free(tmp_koord);
		}
	}
	free(lem->koord);
	lem->koord = NULL;
}

void		delete_default(t_lem *lem)
{
	ft_strdel(&g_input_str);
	delete_rooms(lem);
	delete_koord(lem);
	free(lem);
	lem = NULL;
}
