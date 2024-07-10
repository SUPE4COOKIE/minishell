/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/07/10 03:45:51 by scrumier         ###   ########.fr       */
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
		g_sig = SIGQUIT;
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
	write(1, "\n", 1);
	g_sig = signal;
}

int event(void)
{
	return (0);
}

int main(int argc, char **argv, char **envp)
{
    t_minishell mshell = {0};

	if (PRINT_CAT)
	{
		print_cat();
	}
	allocate_env(&mshell, envp);
	save_path(&mshell, mshell.env); // TODO: protect
	mshell.last_exit_status = 0;
	mshell.last_pid = 0;
	g_sig = 0;
	rl_done = 0;
	mshell.in_heredoc = false;
	if (argc > 1 && strcmp(argv[1], "-c") == 0)
	{
		handle_dash_c(&mshell, argc, argv);
		return (1);
	}
	else
	{
		while (g_sig != SIGQUIT)
		{
			signal(SIGINT, signal_new_line);
			signal(SIGQUIT, signal_new_line);
			rl_event_hook = event;
			mshell.line = readline("minishell$> ");
			signal(SIGINT, signal_exec);
			if (!mshell.line)
				break ;
			if (!is_valid_quotes(mshell.line, &mshell.last_exit_status))
			{
				free(mshell.line);
				continue;
			}
			if (is_n_only_spaces(mshell.line, ft_strlen(mshell.line)) == 1)
			{
				free(mshell.line);
				continue ;
			}
			if (*mshell.line)
				add_history(mshell.line);
			if (parse(&mshell))
				continue ;
			if (exec(&mshell) != 0)
				continue ;
			free(mshell.line);
			mshell.line = NULL;
			free_cmds(mshell.cmds);
		}
	}
	free_mshell(&mshell);
	return (0);
}
