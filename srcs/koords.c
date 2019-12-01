/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   koords.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/01 17:47:52 by pnita             #+#    #+#             */
/*   Updated: 2019/12/01 17:47:54 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem-in.h"

size_t	hash_koord(int x, int y)
{
	size_t	hash;

	hash = x * 5 + y * 7 + 3 * y * x;
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

int		do_with_same_koord(t_lem *lem, int x, int y)
{
	t_koord	*same_koord;

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
	return (1);
}

int		check_koord_only_digit(t_lem *lem, char *str)
{
	char	*ptr;
	int		x;
	int		y;

	ptr = str;
	while (*ptr && *ptr != ' ')
		++ptr;
	if (!*ptr || !*(ptr + 1) || !ft_strn_only_digit(str, ptr - str)
			|| !ft_strn_only_digit(ptr + 1, -1)
			|| (x = ft_atoi(str)) < 0
			|| (y = ft_atoi(ptr + 1)) < 0)
		return (0);
	lem->h_i = hash_koord(x, y);
	if (!lem->koord[lem->h_i])
		lem->koord[lem->h_i] = create_koord(x, y);
	else if (!do_with_same_koord(lem, x, y))
		return (0);
	return (1);
}
