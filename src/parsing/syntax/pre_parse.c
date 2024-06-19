/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 22:55:03 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/20 00:53:42 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
	things to check :
	numbers of quotes
*/

#include "minishell.h"

bool	is_valid_quotes(char *line)
{
	size_t	s_quotes;
	size_t	d_quotes;
	size_t	i;

	s_quotes = 0;
	d_quotes = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
			s_quotes++;
		if (line[i] == '\"')
			d_quotes++;
		i++;
	}
	return ((s_quotes % 2 == 0) && (d_quotes % 2 == 0));
}