#include "minishell.h"

int	is_in_env(char **env, char *key)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_varlen(key, '=');
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len) && env[i][len] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	put_in_env(char **args, size_t i, t_minishell *mshell)
{
	char	*key;
	char	*value;

	key = ft_substr(args[i], 0, ft_varlen(args[i], '='));
	value = ft_strdup(args[i] + ft_varlen(args[i], '=') + 1);
	if (strncmp(key, "PATH=", 5) == 0)
	{
		free_tab(mshell->path);
		save_path(mshell, args + i);
	}
	if (is_in_env(mshell->env, key))
		set_env(&mshell->env, key, value);
	else
		ft_addenv(mshell, key, value);
}

size_t	ft_varlen(char *var, char c)
{
	size_t	i;

	i = 0;
	if (!var)
		return (0);
	while (var[i] && var[i] != c)
		i++;
	return (i);
}

bool	update_existing_env(char ***env, char *key, char *value, int index)
{
	char	*new_env;
	char	*tmp;

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
	(*env)[index] = tmp;
	return (EXIT_SUCCESS);
}

bool	set_env(char ***env, char *key, char *value)
{
	int	i;

	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, ft_strlen(key)) == 0 && (*env)[i][ft_strlen(key)] == '=')
			return (update_existing_env(env, key, value, i));
		i++;
	}
	//printf("key: %s\n", key);
	return (add_new_env_var(env, key, value, i));
}
