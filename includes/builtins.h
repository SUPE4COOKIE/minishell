/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:30:33 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 14:11:28 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_setenv
{
	size_t	len;
}		t_setenv;

typedef struct s_arg
{
	char			*arg;
	struct s_arg	*next;
	struct s_arg	*prev;
}		t_arg;

int		builtin_cd(t_minishell *mshell, char **args);
int		builtin_echo(t_minishell *mshell, char **args);
int		builtin_env(t_minishell *mshell, char **args);
int		builtin_exit(t_minishell *mshell, char **args);
int		builtin_pwd(t_minishell *mshell);
int		builtin_unset(t_minishell *mshell, char **args);
int		builtin_export(t_minishell *mshell, char **args);
int		ft_addenv(t_minishell *mshell, char *key, char *value);
void	remove_nodes(t_arg **head, t_arg *node);
void	free_lst(t_arg *new_args);
char	*lst_to_path(t_arg *new_args);
void	tab_to_lst(t_arg **new_args, char **tab);
bool	update_existing_env(char ***env, char *key, char *value, int index);
bool	add_new_env_var(char ***env, char *key, char *value, int size);
char	*remove_double_point(char *path);
int		change_to_home(t_minishell *mshell);
int		change_to_oldpwd(t_minishell *mshell);
int		change_to_specified(t_minishell *mshell, char *arg, bool is_slash);
bool	change_dir(t_minishell *mshell, char *path);
char	*get_path(char **env, char *key);
bool	set_env(char ***env, char *key, char *value);
int		free_shell(t_minishell *mshell, int status);

#endif