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

/**
 * @brief Convert a list of arguments to a string
 * @param new_args
 * @param path
 * @return
 */
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
		path = args[1];//remove_double_point(args);
		printf("path: %s\n", path);
		if (!path)
			return error_cmd(mshell, 1, "malloc failed");
		result = change_dir(mshell, path);
		free(path);
		return (result);
	}
}
