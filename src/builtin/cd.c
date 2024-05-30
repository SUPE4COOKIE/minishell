/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:59:29 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/30 17:16:51 by scrumier         ###   ########.fr       */
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
bool	set_env(t_minishell *mshell, char *value, char *key)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (mshell->env[i])
	{
		if (ft_strncmp(mshell->env[i], key, ft_strlen(key)) == 0)
		{
			tmp = ft_strjoin(key, "=");
			if (!tmp)
				return (error_cmd(mshell, 1, "malloc failed"));
			free(mshell->env[i]);
			mshell->env[i] = ft_strjoin(tmp, value);
			if (!mshell->env[i])
				return (error_cmd(mshell, 1, "malloc failed"));
			free(tmp);
			return (EXIT_SUCCESS);
		}
		i++;
	}
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
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
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
	char *tmp;
	char cwd[PATH_MAX];

	return_path = NULL;
	if (chdir(path) < 0)
		return (error_cmd(mshell, 1, "cd: no such file or directory"));
	return_path = getcwd(cwd, PATH_MAX);
	if (!return_path)
		return (free(return_path), error_cmd(mshell, 1, "cd: getcwd failed"));
	tmp = get_path(mshell->env, "PWD");
	if (tmp < 0)
		return (free(return_path), error_cmd(mshell, 1, "PWD not set"));
	if (set_env(mshell, "OLDPWD", get_path(mshell->env, "PWD")) == EXIT_FAILURE)
		return (free(return_path), free(tmp), error_cmd(mshell, 1, "OLDPWD not set"));
	if (set_env(mshell, "PWD", return_path) == EXIT_FAILURE)
		return (free(return_path), free(tmp), error_cmd(mshell, 1, "PWD not set"));
	free(return_path);
	free(tmp);
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

	i = 0;
	while (args[i])
	{
		tmp = malloc(sizeof(t_arg));
		if (!tmp)
			return ;
		tmp->arg = ft_strdup(args[i]);
		if (!tmp->arg)
			return (free(tmp));
		tmp->next = NULL;
		tmp->prev = NULL;
		if (!*new_args)
			*new_args = tmp;
		else
		{
			tmp->next = *new_args;
			(*new_args)->prev = tmp;
			*new_args = tmp;
		}
		i++;
	}
}

char *ft_lst_to_char(t_arg *new_args, char *path)
{
	int i;

	i = 0;
	while (new_args)
	{
		ft_strlcpy(path + i, new_args->arg, ft_strlen(new_args->arg) + 1);
		i += ft_strlen(new_args->arg);
		if (new_args->next)
		{
			ft_strlcpy(path + i, "/", 2);
			i++;
		}
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
	char path[PATH_MAX]; // TODO: List_to_char *

	ft_create_list(args, &new_args);
	while (new_args)
	{
		if (new_args->prev && ft_strncmp(new_args->arg, "..", 2) != 0)
		{
			prev_prev = new_args->prev->prev;
			next = new_args->next;
			free(new_args->prev->arg);
			free(new_args->prev);
			free(new_args->arg);
			free(new_args);
			prev_prev->next = next;
			next->prev = prev_prev;
			new_args = next;
		}
		else
			new_args = new_args->next;
	}
	return (ft_lst_to_char(new_args, path));
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

	path = remove_double_point(args);
	if (!args || !args[1] || !args[1][0])
	{
		path = get_path(mshell->env, "HOME");
		if (path < 0)
			return (error_cmd(mshell, 1, "HOME not set"));
		return (change_dir(mshell, path));
	}
	else if (args[2])
		return (error_cmd(mshell, 1, "cd: too many arguments"));
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_path(mshell->env, "OLDPWD");
		if (path < 0)
			return (error_cmd(mshell, 1, "OLDPWD not set"));
		return (change_dir(mshell, path));
	}
	return(change_dir(mshell, args[1]));
}
