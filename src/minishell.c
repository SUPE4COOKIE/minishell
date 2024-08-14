/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/14 10:24:31 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_sig;

void signal_new_line(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig = SIGINT;
	}
	else if (sig == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig = 0;
	}
}

void	signal_here_doc(int signal)
{
	g_sig = signal;
	if (g_sig == SIGINT)
	{
		rl_done = 1;
	}
}

void	signal_exec(int signal)
{
	g_sig = signal;
}

int event(void)
{
	return (0);
}

void init(t_minishell *mshell)
{
	mshell->env = NULL;
	mshell->path = NULL;
	mshell->cmds = NULL;
	mshell->last_exit_status = 0;
	mshell->in_heredoc = false;
	mshell->last_pid = 0;
	mshell->original_stdout = 0;
	mshell->original_stdin = 0;
	mshell->invalid_redir = NULL;
	mshell->cmds = NULL;
	g_sig = 0;
	rl_done = 0;
}

int	print_prompt(t_minishell *mshell)
{
	char *msg;

	msg = ft_strjoin("minishell-\033[0;31m", ft_itoa(mshell->last_exit_status));
	if (!msg)
		return (1);
	msg = ft_strjoin(msg, "\033[0m-$> ");
	if (!msg)
		return (1);
	mshell->line = readline(msg);
	free_null(msg);
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

void	start_minishell(t_minishell *mshell)
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
		if (!mshell->line || g_sig == SIGQUIT)
		{
			free_env_path(mshell);
			printf("exit\n");
			break ;
		}
		if (scan_line(mshell))
			continue ;
		if (parse(mshell))
			continue ;
		if (exec(mshell) != 0)
			continue ;
		free_null(mshell->line);
		mshell->line = NULL;
		free_cmds(mshell->cmds);
	}
}

int main(int argc, char **argv, char **envp)
{
    t_minishell mshell;

	print_cat();
	init(&mshell);
	allocate_env(&mshell, envp);
	save_path(&mshell, mshell.env); // TODO: protect
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		handle_dash_c(&mshell, argc, argv);
		return (mshell.last_exit_status);
	}
	else
		start_minishell(&mshell);
	return (0);
}
