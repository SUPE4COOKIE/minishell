/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonamcrumiere <sonamcrumiere@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 10:23:57 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/05/23 18:06:30 by sonamcrumie      ###   ########.fr       */
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

int	set_env(t_minishell *mshell, const char *name, const char *value, int overwrite)
{
	size_t	name_len;
	size_t	value_len;
	size_t	total_len;
	size_t	env_len;
	char	**new_env;
	char	*new_var;
	int		i;

	if (name == NULL || value == NULL)
		return (EXIT_FAILURE);
    if (mshell->env == NULL) {
        // Allocate space for a new environment with one variable
        mshell->env = (char **)malloc(sizeof(char *) * 2);
        if (mshell->env == NULL) {
            return (EXIT_FAILURE);
        }
        (mshell->env)[0] = NULL;
        (mshell->env)[1] = NULL;
    }
	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	total_len = name_len + value_len + 2; // +2 for '=' and '\0'
	i = 0;
	while (mshell->env[i])
	{
		if (strncmp(mshell->env[i], name, name_len) == 0 && mshell->env[i][name_len] == '=') // may be name_len + 1
		{
			
			if (overwrite)
			{
				new_var = (char *)malloc(sizeof(char *) * total_len);
				if (new_var == NULL)
					return (error_msg_free("set_env: malloc failed", NULL, NULL, NULL));
				ft_strlcpy(new_var, name, name_len + 1);
				ft_strlcat(new_var, "=", name_len + 2);
				ft_strlcat(new_var, value, total_len);
				free(mshell->env[i]);
				mshell->env[i] = ft_strjoin(name, value);
			}
			return (EXIT_SUCCESS);
		}
		i++;
	}
	// the var does not exist
	env_len = count_env(mshell->env);
	new_env = (char **)malloc(sizeof(char *) * (env_len + 2));
	if (new_env == NULL)
		return (error_msg_free("set_env: malloc failed", new_env, NULL, NULL));
	i = 0;
	while (i < env_len)
	{
		new_env[i] = mshell->env[i];
		i++;
	}
	new_var = (char *)malloc(sizeof(char *) * total_len);
	if (new_var == NULL)
		return (EXIT_FAILURE);
	ft_strlcpy(new_var, name, name_len + 1);
	ft_strlcat(new_var, "=", name_len + 2);
	ft_strlcat(new_var, value, total_len);
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(mshell->env);
	mshell->env = new_env;
	return (EXIT_SUCCESS);
}
