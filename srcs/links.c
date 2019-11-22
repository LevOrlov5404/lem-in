/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   links.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/10 14:35:35 by pnita             #+#    #+#             */
/*   Updated: 2019/11/10 14:35:37 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

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

void		add_way_link(t_r *r, char *name)
{
	t_l	*tmp_link;
	
	if (!r->way_links)
		r->way_links = create_link(name);
	else
	{
		tmp_link = r->way_links;
		while (tmp_link->next)
			tmp_link = tmp_link->next;
		tmp_link->next = create_link(name);
	}
}

void		delete_way_link(t_r *room)
{
	t_l *wl;
	t_l *tmp_wl;

	wl = room->way_links;
	while (wl)
	{
		tmp_wl = wl;
		wl = wl->next;
		ft_strdel(&tmp_wl->name);
		free(tmp_wl);
		tmp_wl = NULL;
	}
	room->way_links = NULL;
}
