/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 16:52:52 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 16:58:46 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arg	*create_new_arg(char *arg)
{
	t_arg	*new_arg;

	new_arg = malloc(sizeof(t_arg));
	if (!new_arg)
		return (NULL);
	new_arg->arg = ft_strdup(arg);
	if (!new_arg->arg)
	{
		free(new_arg);
		return (NULL);
	}
	new_arg->next = NULL;
	new_arg->prev = NULL;
	return (new_arg);
}

void	append_arg(t_arg **new_args, t_arg *new_arg)
{
	t_arg	*tmp;

	if (!*new_args)
		*new_args = new_arg;
	else
	{
		tmp = *new_args;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_arg;
		new_arg->prev = tmp;
	}
}

void	tab_to_lst(t_arg **new_args, char **tab)
{
	t_arg	*new_arg;
	int		i;

	i = 0;
	while (tab[i])
	{
		new_arg = create_new_arg(tab[i]);
		if (!new_arg)
			return ;
		append_arg(new_args, new_arg);
		i++;
	}
}

char	*lst_to_path(t_arg *new_args)
{
	t_arg	*tmp;
	char	*path;
	char	*tmp_path;

	path = NULL;
	tmp = new_args;
	while (tmp)
	{
		if (!path)
		{
			path = ft_strdup(tmp->arg);
		}
		else
		{
			tmp_path = ft_strjoin(path, "/");
			free_null(path);
			path = ft_strjoin(tmp_path, tmp->arg);
			free_null(tmp_path);
		}
		tmp = tmp->next;
	}
	return (path);
}

void	remove_nodes(t_arg **head, t_arg *node)
{
	if (node == *head)
	{
		*head = node->next;
		if (*head)
			(*head)->prev = NULL;
	}
	else
	{
		if (node->prev)
			node->prev->next = node->next;
		if (node->next)
			node->next->prev = node->prev;
	}
	if (node->arg)
		free_null(node->arg);
	free_null(node);
}
