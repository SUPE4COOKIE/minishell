/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   name_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 12:21:56 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/13 12:52:21 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *tmp_file(int nb)
{
	char *tmp;
	char *file_name;

	file_name = ft_itoa(nb);
	if (file_name)
	{
		tmp = file_name;
		file_name = ft_strjoin("/tmp/", file_name);
		free_null(tmp);
		if (file_name)
			return (file_name);
	}
	return (NULL);
}