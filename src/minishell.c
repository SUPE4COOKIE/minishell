/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/16 12:01:09 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig;

int	scan_line(t_minishell *mshell)
{
	if (!is_valid_quotes(mshell->line, &mshell->last_exit_status))
	{
		free_null(mshell->line);
		return (1);
	}
	if (is_n_only_spaces(mshell->line, ft_strlen(mshell->line)) == 1)
	{
		free_null(mshell->line);
		return (1);
	}
	if (mshell->line)
		add_history(mshell->line);
	return (0);
}

int	start_mshell(t_minishell *mshell)
{
	if (scan_line(mshell))
		return (1);
	if (parse(mshell))
		return (1);
	if (exec(mshell) != 0)
	{
		free_cmds(mshell->cmds);
		return (1);
	}
	return (0);
}

void	prompt_minishell(t_minishell *mshell)
{
	while (42)
	{
		signal(SIGINT, signal_new_line);
		signal(SIGQUIT, signal_new_line);
		rl_event_hook = event;
		mshell->line = readline("minishell$ ");
		if (mshell->line == NULL)
		{
			free_env_path(mshell);
			break ;
		}
		signal(SIGINT, signal_exec);
		signal(SIGQUIT, signal_exec);
		if (!mshell->line || g_sig == SIGQUIT)
		{
			free_env_path(mshell);
			printf("exit\n");
			break ;
		}
		if (start_mshell(mshell) == 1)
			continue ;
		free_null(mshell->line);
		free_cmds(mshell->cmds);
	}
}

int	main(int argc, char **envp)
{
	t_minishell	mshell;

	if (argc != 1)
		return(1);
	print_cat();
	init(&mshell);
	allocate_env(&mshell, envp);
	mshell.last_exit_status = 0;
	if (save_path(&mshell, mshell.env))
	{
		free_env_path(&mshell);
		return (mshell.last_exit_status);
	}
	else if (argc == 1)
		prompt_minishell(&mshell);
	return (mshell.last_exit_status);
}
