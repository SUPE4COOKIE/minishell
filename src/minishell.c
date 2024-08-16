/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/15 11:04:44 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig;

int	print_prompt(t_minishell *mshell)
{
	char	*msg;
	char	*tmp;

	tmp = ft_itoa(mshell->last_exit_status);
	if (!tmp)
		return (1);
	msg = ft_strjoin("minishell-\033[0;31m", tmp);
	if (!msg)
		return (1);
	free_null(tmp);
	tmp = ft_strjoin(msg, "\033[0m-$> ");
	if (!tmp)
		return (1);
	mshell->line = readline(tmp);
	free_null(msg);
	free_null(tmp);
	return (0);
}

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
		if (print_prompt(mshell))
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

int	main(int argc, char **argv, char **envp)
{
	t_minishell	mshell;

	print_cat();
	init(&mshell);
	allocate_env(&mshell, envp);
	mshell.last_exit_status = 0;
	if (save_path(&mshell, mshell.env))
	{
		free_env_path(&mshell);
		return (mshell.last_exit_status);
	}
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		handle_dash_c(&mshell, argc, argv);
		return (mshell.last_exit_status);
	}
	else if (argc == 1)
		prompt_minishell(&mshell);
	return (mshell.last_exit_status);
}
