/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonamcrumiere <sonamcrumiere@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 10:23:57 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/05/25 10:51:55 by sonamcrumie      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

int set_env_var(t_minishell *mshell, const char *name, const char *value, int overwrite, t_setenv env, int i)
{
	char *new_var;

	if (strncmp(mshell->env[i], name, env.name_len) == 0 && mshell->env[i][env.name_len] == '=') {
		if (overwrite) {
			new_var = (char *)malloc(sizeof(char *) * env.total);
			if (new_var == NULL)
				return (error_msg_free("set_env: malloc failed", NULL, NULL, NULL));
			ft_strlcpy(new_var, name, env.name_len + 1);
			ft_strlcat(new_var, "=", env.name_len + 2);
			ft_strlcat(new_var, value, env.total);
			free(mshell->env[i]);
			mshell->env[i] = ft_strjoin(name, value);
		}
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}



int	builtin_export(t_minishell *mshell, const char *name, \
		const char *value, int overwrite)
{
	t_setenv	env;
	char	**new_env;
	char	*new_var;
	int		i;

	if (name == NULL || value == NULL)
		return (EXIT_FAILURE);
    if (mshell->env == NULL) {
        mshell->env = (char **)malloc(sizeof(char *) * 2);
        if (mshell->env == NULL) {
            return (EXIT_FAILURE);
        }
        (mshell->env)[0] = NULL;
        (mshell->env)[1] = NULL;
    }
	env.name_len = ft_strlen(name);
	env.value_len = ft_strlen(value);
	env.total = env.name_len + env.value_len + 2; // +2 for '=' and '\0'
	i = 0;
	while (mshell->env[i]) {
		if (set_env_var(mshell, name, value, overwrite, env, i) == EXIT_SUCCESS) {
			return (EXIT_SUCCESS);
		}
		i++;
	}
	// the var does not exist
	env.env = count_env(mshell->env);
	new_env = (char **)malloc(sizeof(char *) * (env.env + 2));
	if (new_env == NULL)
		return (error_msg_free("set_env: malloc failed", new_env, NULL, NULL));
	i = 0;
	while (i < env.env)
	{
		new_env[i] = mshell->env[i];
		i++;
	}
	new_var = (char *)malloc(sizeof(char *) * env.total);
	if (new_var == NULL)
		return (EXIT_FAILURE);
	ft_strlcpy(new_var, name, env.name_len + 1);
	ft_strlcat(new_var, "=", env.name_len + 2);
	ft_strlcat(new_var, value, env.total);
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(mshell->env);
	mshell->env = new_env;
	return (EXIT_SUCCESS);
}
