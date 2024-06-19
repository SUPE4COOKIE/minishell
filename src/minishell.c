/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/14 17:14:13 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void signal_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
		return ;
}

void readline_event_hook(void)
{
//	if (g_in_hdoc == true)
//		rl_done = 1;
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
    t_minishell mshell = {0};
	struct sigaction sa;

	allocate_env(&mshell, envp);
	mshell.last_exit_status = 0;
	mshell.in_heredoc = false;
	rl_event_hook = (int (*)(void)) readline_event_hook;
	sa.sa_sigaction = &signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1 ||
		sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		printf("Error: signal\n");
		return (1);
	}
	if (argc == 3 && ft_strncmp(argv[1], "-c", 2) == 0)
	{
		mshell.line = ft_strdup(argv[2]);
		mshell.line[ft_strlen(mshell.line) - 1] = 0;
		parse(&mshell);
		exec(&mshell);
		free(mshell.line);
		mshell.last_exit_status = 0;
		return (0);
	}
	else
	{
		(void)argv;
		while (42)
		{
			mshell.line = readline("minishell$ ");
			if (!mshell.line) {
				free_shell(&mshell, 0);
				printf("\n");
				exit(0);
			}
			if (is_n_only_spaces(mshell.line, ft_strlen(mshell.line)) == 1)
			{
				free(mshell.line);
				continue ;
			}
			if (*mshell.line) {
				add_history(mshell.line);
			}
			parse(&mshell);
			exec(&mshell);
			free(mshell.line);
		}
	}
	return (0);
}
