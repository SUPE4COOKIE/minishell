/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:01:31 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/14 10:00:20 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	change_dir(t_minishell *mshell, char *path)
{
	char	cwd[PATH_MAX];

	if (chdir(path) != 0)
		return (perror("minishell: cd"), 1);
	if (getcwd(cwd, PATH_MAX) == NULL)
		return (perror("minishell: cd"), 1);
	if (set_env(&mshell->env, "OLDPWD", \
			get_path(mshell->env, "PWD")) == EXIT_FAILURE)
		return (perror("minishell: cd"), 1);
	if (set_env(&mshell->env, "PWD", cwd) == EXIT_FAILURE)
		return (perror("minishell: cd"), 1);
	return (EXIT_SUCCESS);
}

int	change_to_home(t_minishell *mshell)
{
	char	*path;

	path = get_path(mshell->env, "HOME");
	if (!path)
		return (error_cmd(mshell, 1, "HOME not set"));
	return (change_dir(mshell, path));
}

int	change_to_oldpwd(t_minishell *mshell)
{
	char	*path;

	path = get_path(mshell->env, "OLDPWD");
	if (!path)
		return (1);
	return (change_dir(mshell, path));
}

static void	add_slash_cd(char *path)
{
	path[0] = '/';
	path[1] = '\0';
}

int	change_to_specified(t_minishell *mshell, char *arg, bool is_slash)
{
	char	*path;
	int		result;

	result = 0;
	path = remove_double_point(arg);
	if (is_slash == true)
	{
		if (!path)
		{
			path = malloc(2 * sizeof(char));
			if (!path)
				return (error_msg("malloc failed"));
			add_slash_cd(path);
		}
		else
		{
			path = ft_strjoin("/", path);
			if (!path)
				return (error_msg("ft_strjoin failed"));
		}
	}
	if (!path)
		return (0);
	result = change_dir(mshell, path);
	if (result == 1)
		mshell->last_exit_status = 1;
	return (free_null(path), result);
}
