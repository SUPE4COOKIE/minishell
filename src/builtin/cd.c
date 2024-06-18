/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:59:29 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/14 15:51:31 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_print_lst(t_arg *new_args)
{
	t_arg *tmp;

	tmp = new_args;
	while (tmp)
	{
		printf("arg: %s\n", tmp->arg);
		tmp = tmp->next;
	}
}

/**
** @brief Set the value of a key in the env
** @param mshell The minishell structure
** @param value The value to set
** @param key The key to set
** @return 0 if success, 1 if error
*/
bool	set_env(char ***env, char *value, char *key)
{
	int		i;
	char	*new_env;
	char	*tmp;
	char	**new_env_tab;
	int		j;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, ft_strlen(key)) == 0 && (*env)[i][ft_strlen(key)] == '=')
		{
			new_env = ft_strjoin(key, "=");
			if (!new_env)
				return (EXIT_FAILURE);
			tmp = ft_strjoin(new_env, value);
			if (!tmp)
			{
				free(new_env);
				return (EXIT_FAILURE);
			}
			free(new_env);
			(*env)[i] = tmp;
			return (EXIT_SUCCESS);
		}
		i++;
	}
	// add the var in the env
	new_env = ft_strjoin(key, "=");
	if (!new_env)
		return (EXIT_FAILURE);
	tmp = ft_strjoin(new_env, value);
	if (!tmp)
	{
		free(new_env);
		return (EXIT_FAILURE);
	}
	free(new_env);
	new_env_tab = malloc((i + 2) * sizeof(char *));
	if (!new_env_tab)
	{
		free(tmp);
		return (EXIT_FAILURE);
	}
	j = 0;
	while (j < i)
	{
		new_env_tab[j] = (*env)[j];
		j++;
	}
	new_env_tab[i] = tmp;
	new_env_tab[i + 1] = NULL;

	//free(*env);
	*env = new_env_tab;
	return (EXIT_SUCCESS);
}

/**
** @brief Get the path of a key in the env
** @param env The env
** @param key The key to search
** @return The path if success, NULL if error
*/
char	*get_path(char **env, char *key)
{
	int		i;
	char	*path;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0 && env[i][ft_strlen(key)] == '=')
		{
			path = ft_strchr(env[i], '=') + 1;
			return (path);
		}
		i++;
	}
	return (NULL);
}

/**
** @brief Change the current directory
** @param mshell The minishell structure
** @param path The path to change to
** @return 0 if success, 1 if error
*/
static bool	change_dir(t_minishell *mshell, char *path)
{
	char cwd[PATH_MAX];

	//if (DEBUG == true)
		printf("path: %s\n", path);
	if (chdir(path) != 0)
		return (error_cmd(mshell, 1, "cd: no such file or directory"));
	getcwd(cwd, PATH_MAX);
	if (cwd[0] == '\0')
		return (error_cmd(mshell, 1, "cd: getcwd failed"));
	if (set_env(&mshell->env, get_path(mshell->env, "PWD"), "OLDPWD") == EXIT_FAILURE)
		return (error_cmd(mshell, 1, "cd: setenv failed"));
	if (set_env(&mshell->env, cwd, "PWD") == EXIT_FAILURE) {
		return (error_cmd(mshell, 1, "cd: setenv failed"));
	}
	//if (DEBUG == true)
		printf("return_path: %s\n", cwd);
	return (EXIT_SUCCESS);
}

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

char *lst_to_path(t_arg *new_args)
{
	t_arg *tmp;
	char *path;
	char *tmp_path;

	path = NULL;
	tmp = new_args;
	if (!tmp)
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
	if (node == *head) {
		*head = node->next;
		if (*head) {
			(*head)->prev = NULL;
		}
	} else {
		if (node->prev) {
			node->prev->next = node->next;
		}
		if (node->next) {
			node->next->prev = node->prev;
		}
	}
	if (node->arg) {
		free(node->arg);
	}
	free(node);
}

char *remove_double_point(char *path)
{
	t_arg *new_args;
	t_arg *tmp;
	t_arg *to_remove;
	char **tab;
	char *new_path;

	new_args = NULL;
	tab = ft_split(path, '/');
	if (!tab)
		return (NULL);
	tab_to_lst(&new_args, tab);
	tmp = new_args;
	while (tmp)
	{
		if (tmp->prev && ft_strncmp(tmp->prev->arg, "..", 3) != 0 && ft_strncmp(tmp->arg, "..", 3) == 0)
		{
			to_remove = tmp;
			tmp = tmp->prev;
			remove_nodes(&new_args, to_remove);
			to_remove = tmp;
			tmp = tmp->prev;
			remove_nodes(&new_args, to_remove);
		}
		tmp = tmp->next;
	}
	new_path = lst_to_path(new_args);
	free_tab(tab);
	free_lst(new_args);
	return (new_path);
}

/**
** @brief Change the current directory
** @param mshell The minishell structure
** @param args The arguments of cd command
** @return 0 if success, 1 if error
*/
int	builtin_cd(t_minishell *mshell, char **args)
{
	char   *path;
	int result;

	if (!args || !args[1] || !args[1][0])
	{
		path = get_path(mshell->env, "HOME");
		if (!path)
			return (error_cmd(mshell, 1, "HOME not set"));
		return (change_dir(mshell, path));
	}
	else if (args[2])
		return (error_cmd(mshell, 1, "cd: too many arguments"));
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_path(mshell->env, "OLDPWD");
		if (!path)
			return (error_cmd(mshell, 1, "OLDPWD not set"));
		return (change_dir(mshell, path));
	}
	else
	{
		path = remove_double_point(args[1]);
		printf("path: %s\n", path);
		if (!path)
			return error_cmd(mshell, 1, "malloc failed");
		result = change_dir(mshell, path);
		free(path);
		return (result);
	}
}
