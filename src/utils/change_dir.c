#include "minishell.h"

bool	change_dir(t_minishell *mshell, char *path)
{
	char	cwd[PATH_MAX];

	if (DEBUG == true)
		printf("path: %s\n", path);
	if (chdir(path) != 0)
		return (error_cmd(mshell, 1, "cd: no such file or directory"));
	getcwd(cwd, PATH_MAX);
	if (cwd[0] == '\0')
		return (error_cmd(mshell, 1, "cd: getcwd failed"));
	if (set_env(&mshell->env, get_path(mshell->env, "PWD"), \
			"OLDPWD") == EXIT_FAILURE)
		return (error_cmd(mshell, 1, "cd: setenv failed"));
	if (set_env(&mshell->env, cwd, "PWD") == EXIT_FAILURE)
		return (error_cmd(mshell, 1, "cd: setenv failed"));
	if (DEBUG == true)
		printf("return_path: %s\n", cwd);
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
		return (error_cmd(mshell, 1, "OLDPWD not set"));
	return (change_dir(mshell, path));
}

int	change_to_specified(t_minishell *mshell, char *arg)
{
	char	*path;
	int		result;

	path = remove_double_point(arg);
	if (!path)
		return (0);
	result = change_dir(mshell, path);
	free(path);
	return (result);
}