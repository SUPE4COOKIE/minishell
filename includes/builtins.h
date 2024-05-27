/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonamcrumiere <sonamcrumiere@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:30:33 by scrumier          #+#    #+#             */
/*   Updated: 2024/05/25 10:51:20 by sonamcrumie      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_setenv
{
	size_t		name_len;
	size_t		value_len;
	size_t		total;
	size_t		env;
	const char	*name;
	const char	*value;
	int			overwrite;
}		t_setenv;

int	builtin_cd(t_minishell *mshell, char **args);
int	builtin_echo(t_minishell *mshell, char **args);
int	builtin_env(t_minishell *mshell, char **args);
int	builtin_export(t_minishell *mshell, const char *name, \
		const char *value, int overwrite);

#endif