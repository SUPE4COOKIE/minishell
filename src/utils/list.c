#include "minishell.h"

void tab_to_lst(t_arg **new_args, char **tab)
{
	t_arg *new_arg;
	t_arg *tmp;
	int i;

	i = 0;
	while (tab[i])
	{
		new_arg = malloc(sizeof(t_arg));
		if (!new_arg)
			return ;
		new_arg->arg = ft_strdup(tab[i]);
		if (!new_arg->arg)
		{
			free(new_arg);
			return ;
		}
		new_arg->next = NULL;
		new_arg->prev = NULL;
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
		i++;
	}
}

char *lst_to_path(t_arg *new_args, bool is_absolute)
{
	t_arg *tmp;
	char *path;
	char *tmp_path;

	path = NULL;
	tmp = new_args;
	if (!tmp || is_absolute == true)
		path = ft_strdup("/");
	while (tmp)
	{
		if (!path)
		{
			path = ft_strdup(tmp->arg);
		}
		else
		{
			tmp_path = ft_strjoin(path, "/");
			free(path);
			path = ft_strjoin(tmp_path, tmp->arg);
			free(tmp_path);
		}
		tmp = tmp->next;
	}
	return (path);
}

void free_lst(t_arg *new_args)
{
	t_arg *tmp;

	while (new_args)
	{
		tmp = new_args;
		new_args = new_args->next;
		free(tmp->arg);
		free(tmp);
	}
}

void remove_nodes(t_arg **head, t_arg *node)
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
		free(node->arg);
	free(node);
}
