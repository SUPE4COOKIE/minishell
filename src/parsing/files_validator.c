/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_validator.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 07:18:03 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/24 07:19:27 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	to add:
		- support for directories
		- support for ~
		- support for ./ and ../
*/

bool	is_valid_file(char *file)
{
	if (access(file, F_OK) == 0)
		return (true);
	return (false);
}