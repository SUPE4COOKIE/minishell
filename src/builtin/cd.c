/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonam <sonam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:59:29 by scrumier          #+#    #+#             */
/*   Updated: 2024/06/04 14:27:47 by sonam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** @brief Set the value of a key in the env
** @param mshell The minishell structure
** @param value The value to set
** @param key The key to set
** @return 0 if success, 1 if error
*/
bool	set_env(t_minishell *mshell, char *value, char *key) {
	int i;
	char *tmp;
	char *new_value;

	i = 0;
	while (mshell->env[i]) {
		if (ft_strncmp(mshell->env[i], key, ft_strlen(key)) == 0 && mshell->env[i][ft_strlen(key)] == '=')
		{
			tmp = ft_strjoin(key, "=");
			if (!tmp)
				return error_cmd(mshell, 1, "malloc failed");
			new_value = ft_strjoin(tmp, value);
			free(tmp);
			if (!new_value)
				return error_cmd(mshell, 1, "malloc failed");
			free(mshell->env[i]);
			mshell->env[i] = new_value;
			return (EXIT_SUCCESS);
		}
		i++;
	}
	// Code to add a new environment variable if it doesn't exist
	// Not provided in original code
	return (EXIT_FAILURE);
}

/*
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

/*
** @brief Change the current directory
** @param mshell The minishell structure
** @param path The path to change to
** @return 0 if success, 1 if error
*/
static bool	change_dir(t_minishell *mshell, char *path)
{
	char *return_path;
	char cwd[PATH_MAX];

	return_path = NULL;
	if (chdir(path) < 0)
		return (error_cmd(mshell, 1, "cd: no such file or directory"));
	return_path = getcwd(cwd, PATH_MAX);
	if (!return_path)
		return (error_cmd(mshell, 1, "cd: getcwd failed"));
	if (set_env(mshell, get_path(mshell->env, "PWD"), "OLDPWD") == EXIT_FAILURE)
		return (error_cmd(mshell, 1, "cd: setenv failed"));
	printf("return_path: %s\npath: %s\n", return_path, path);
	if (set_env(mshell, return_path, "PWD") == EXIT_FAILURE)
		return (error_cmd(mshell, 1, "cd: setenv failed"));
	return (EXIT_SUCCESS);
}

/*
** @brief Create a list of arguments
** @param args The arguments
** @param new_args The new list
*/
void ft_create_list(char **args, t_arg **new_args)
{
	int i;
	t_arg *tmp;
	t_arg *current;
	t_arg *last;

	i = 0;
	while (args[i])
	{
		tmp = malloc(sizeof(t_arg));
		if (!tmp)
		{
			current = *new_args;
			while (current) {
				t_arg *next = current->next;
				free(current->arg);
				free(current);
				current = next;
			}
			return ;
		}
		tmp->arg = ft_strdup(args[i]);
		if (!tmp->arg)
		{
			free(tmp);
			current = *new_args;
			while (current) {
				t_arg *next = current->next;
				free(current->arg);
				free(current);
				current = next;
			}
			return;
		}
		tmp->next = NULL;
		tmp->prev = NULL;
		if (!*new_args)
			*new_args = tmp;
		else
		{
			last = *new_args;
			while (last->next)
				last = last->next;
			last->next = tmp;
			tmp->prev = last;
		}
		i++;
	}
}

char *ft_lst_to_char(t_arg *new_args, char *path)
{
	if (!path)
		return (NULL);
	path[0] = '\0';
	while (new_args)
	{
		ft_strlcat(path, new_args->arg, PATH_MAX);
		if (new_args->next)
			ft_strlcat(path, "/", PATH_MAX);
		new_args = new_args->next;
	}
	return (path);
}

/*
** @brief Remove the double point in the path
** @param args The arguments
*/
char *remove_double_point(char **args)
{
	t_arg	*new_args;
	t_arg	*prev_prev;
	t_arg	*next;
	t_arg	*current;
	char *path;

	path = malloc(sizeof(char) * PATH_MAX);
	if (!path)
		return (NULL);
	ft_create_list(args, &new_args);
	//print list
	t_arg *tmp = new_args;
	while (tmp)
	{
		printf("%s\n", tmp->arg);
		tmp = tmp->next;
	}
	current = new_args;
	while (current)
	{
		if (current->prev && ft_strncmp(current->arg, "..", 2) != 0)
		{
			prev_prev = current->prev->prev;
			next = current->next;
			free(current->prev->arg);
			free(current->prev);
			free(current->arg);
			free(current);
			if (prev_prev)
				prev_prev->next = next;
			if (next)
				next->prev = prev_prev;
			current = next;
		}
		else
			current = current->next;
	}
	path = ft_lst_to_char(current, path);
	current = new_args;
	while (current)
	{
		next = current->next;
		free(current->arg);
		free(current);
		current = next;
	}
	return (path);
}

/*
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
		path = remove_double_point(args);
		if (!path)
			return error_cmd(mshell, 1, "malloc failed");
		result = change_dir(mshell, path);
		free(path);
		return (result);
	}
}
