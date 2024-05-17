/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:59:29 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/17 13:35:53 by scrumier         ###   ########.fr       */
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
	while (mshell->env[i])
	{
		if (ft_strncmp(mshell->env[i], key, ft_strlen(key)) == 0)
		{
			tmp = ft_strjoin(key, "=");
			if (!tmp)
				return (error_cmd(&mshell, 1, "malloc failed"));
			free(mshell->env[i]);
			mshell->env[i] = ft_strjoin(tmp, value);
			if (!mshell->env[i])
				return (error_cmd(&mshell, 1, "malloc failed"));
			free(tmp);
			return (0);
		}
		i++;
	}
	if (tmp)
		free(tmp);
	return (1);
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
	if (path)
		free(path);
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
		return (error_cmd(&mshell, 1, "cd: no such file or directory"));
	return_path = getcwd(cwd, PATH_MAX);
	if (!return_path)
		return (free(return_path), error_cmd(&mshell, 1, "cd: getcwd failed")); // TODO : implement free_ptr
	tmp = get_path(&mshell->env, "PWD");
	if (tmp < 0)
		return (free(return_path), error_cmd(&mshell, 1, "PWD not set"));
	if (set_env(&mshell, "OLDPWD", get_path(mshell->env, "PWD")) < 0)
		return (free(return_path), free(tmp), error_cmd(&mshell, 1, "OLDPWD not set"));
	if (set_env(&mshell, "PWD", return_path) < 0)
		return (free(return_path), free(tmp), error_cmd(&mshell, 1, "PWD not set"));
	free(return_path);
	free(tmp);
	return (0);
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

	if (!args || !args[1] || !args[1][0])
	{
		path = get_path(mshell->env, "HOME"); // TODO : implement get_path
		if (path < 0)
			return (error_cmd(&mshell, 1, "HOME not set"));
		return (change_dir(&mshell, path));
	}
	else if (args[2])
		return (error_cmd(&mshell, 1, "cd: too many arguments"));
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_path(mshell->env, "OLDPWD");
		if (path < 0)
			return (error_cmd(&mshell, 1, "OLDPWD not set"));
		return (change_dir(&mshell, path));
	}
	else
		return(change_dir(&mshell, args[1]));
	return (0);
}
