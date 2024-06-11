/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:30:33 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/30 15:07:56 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_setenv
{
	size_t 	len;
}		t_setenv;

typedef struct s_arg
{
	char	*arg;
	struct s_arg	*next;
	struct s_arg	*prev;
}		t_arg;

int	builtin_cd(t_minishell *mshell, char **args);
int	builtin_echo(t_minishell *mshell, char **args);
int	builtin_env(t_minishell *mshell, char **args);
int	builtin_exit(t_minishell *mshell, char **args);
int	builtin_pwd(t_minishell *mshell);
void	builtin_unset(t_minishell *mshell, char **args);
bool	set_env(char ***env, char *value, char *key);
int	builtin_export(t_minishell *mshell, char **args);

#endif