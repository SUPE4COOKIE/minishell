/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:59:29 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/13 15:43:30 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	add_new_env_var(char ***env, char *key, char *value, int size)
{
	char	*new_env;
	char	*tmp;
	char	**new_env_tab;
	int		j;

	new_env = ft_strjoin(key, "=");
	if (!new_env)
		return (EXIT_FAILURE);
	tmp = ft_strjoin(new_env, value);
	if (!tmp)
		return (free(new_env), EXIT_FAILURE);
	free_null(new_env);
	new_env_tab = malloc((size + 2) * sizeof(char *));
	if (!new_env_tab)
		return (free(tmp), EXIT_FAILURE);
	j = -1;
	while (++j < size)
		new_env_tab[j] = (*env)[j];
	new_env_tab[size] = tmp;
	new_env_tab[size + 1] = NULL;
	free_null(*env);
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
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0 && \
				env[i][ft_strlen(key)] == '=')
		{
			path = ft_strchr(env[i], '=') + 1;
			return (path);
		}
		i++;
	}
	return (NULL);
}

int	builtin_cd(t_minishell *mshell, char **args)
{
	bool	is_slash;

	if (!args || !args[1] || !args[1][0])
		return (change_to_home(mshell));
	if (args[2])
		return (error_cmd(mshell, 1, "cd: too many arguments"));
	if (ft_strncmp(args[1], "-", 2) == 0)
		return (change_to_oldpwd(mshell));
	if (args[1][0] == '/')
		is_slash = true;
	else
		is_slash = false;
	return (change_to_specified(mshell, args[1], is_slash));
}
