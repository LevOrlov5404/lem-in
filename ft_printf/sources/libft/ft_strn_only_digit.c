/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdigit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnita <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 18:37:09 by pnita             #+#    #+#             */
/*   Updated: 2019/11/26 18:37:10 by pnita            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strn_only_digit(char *str, int n)
{
	char	*ptr;

	ptr = str;
	while (*ptr)
	{
		if (!ft_isdigit(*ptr))
			return (0);
		++ptr;
		if (n != -1 && ptr == str + n)
			break ;
	}
	return (1);
}
