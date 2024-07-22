/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_double_points.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:56:17 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 16:56:53 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arg	*path_to_list(char *path)
{
	t_arg	*new_args;
	char	**tab;

	new_args = NULL;
	tab = ft_split(path, '/');
	if (!tab)
		return (NULL);
	tab_to_lst(&new_args, tab);
	free_tab(tab);
	return (new_args);
}

void	process_list(t_arg **new_args)
{
	t_arg	*tmp;
	t_arg	*to_remove;

	tmp = *new_args;
	while (tmp)
	{
		if (tmp->prev && ft_strncmp(tmp->prev->arg, "..", 3) != 0 && \
			ft_strncmp(tmp->arg, "..", 3) == 0)
		{
			to_remove = tmp;
			tmp = tmp->prev;
			remove_nodes(new_args, to_remove);
			to_remove = tmp;
			tmp = tmp->prev;
			remove_nodes(new_args, to_remove);
		}
		else
			tmp = tmp->next;
	}
}

char	*remove_double_point(char *path)
{
	t_arg	*new_args;
	char	*new_path;

	new_args = path_to_list(path);
	if (!new_args)
		return (NULL);
	process_list(&new_args);
	if (new_args != NULL)
		new_path = lst_to_path(new_args);
	else
	{
		free(path);
		new_path = NULL;
	}
	free_lst(new_args);
	return (new_path);
}
