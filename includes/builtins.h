/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:30:33 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/29 14:27:52 by mwojtasi         ###   ########.fr       */
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
int	builtin_export(t_minishell *mshell, const char *name, \
		const char *value, int overwrite);
int	builtin_pwd(t_minishell *mshell);

#endif