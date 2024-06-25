/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/25 12:17:53 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool g_in_hdoc;

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
//	else
//		rl_done = 0;
}

int main(int argc, char **argv, char **envp)
{
    t_minishell mshell = {0};
	struct sigaction sa;

	if (PRINT_CAT)
	{
		ft_printf("    ░▒▒▒▓▒▒░▒██▓ ▓▓██▒▒▒░▓▒▒░░░ ░░▒▒▓▓▓▒░    \n");
		ft_printf("    ▓▓▓▒▒▒▒░░░▒▒▒████▒▒▒▒▒▒░░   ░▒▒▒▒▒░      \n");
		ft_printf("     ██▓▒▒░░▒▒▓██████▒░░░░░▒▒░ ░░░▒▒         \n");
		ft_printf("        ░░░░▒░░▒██████▒░░░   ░▒░░░░          \n");
		ft_printf("        ▒░░░░   ▒██████▒▒  ░ ▒▒░░░░          \n");
		ft_printf("         ░░▒░    ██████▒░░   ▓░░░░           \n");
		ft_printf("         ▒░░▒▒▒▒▒███████▒▒▓▒▒░  ░            \n");
		ft_printf("         ▒░░░▓████████████▓▓░  ░░            \n");
		ft_printf("          ▒▒▓████████████████▒ ░             \n");
		ft_printf("          ███████████████████▓▓░             \n");
		ft_printf("        ██▓▒▒██████▓▓█▓██████████            \n");
		ft_printf("        ▓▒▒▓▓████████▓████████████           \n");
		ft_printf("       ▒▒▒▒▒▒███████▓▒▓██████████            \n");
		ft_printf("       ▒▒░▒░▒▓██████████████████             \n");
		ft_printf("      ░ ░░░░░▓██████████████████             \n");
		ft_printf("     ░░   ░░░░▓█████████████████             \n");
		ft_printf("      ░    ░░░▒████████████████              \n");
		ft_printf("      ░░░░   ░▒███████████████               \n");
		ft_printf("              ░▓██████████████               \n");
		ft_printf("               ▒██████████████               \n");
		ft_printf("               ░░░▒██      █████             \n");
		ft_printf("                ░ ░▒▒       ██████           \n");
		ft_printf("                 ░░ ░▒                       \n");
	}
	allocate_env(&mshell, envp);
	mshell.last_exit_status = 0;
	g_in_hdoc = false;
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
	if (argc > 1 && strcmp(argv[1], "-c") == 0)
	{
		mshell.line = ft_strdup(argv[2]);
		mshell.line[ft_strlen(mshell.line) - 1] = 0;
		if (!is_valid_quotes(mshell.line, &mshell.last_exit_status))
		{
			free_mshell(&mshell);
			return (1);
		}
		if (parse(&mshell))
		{
			free_mshell(&mshell);
			return (1);
		}
		exec(&mshell);
		mshell.last_exit_status = 0;
	}
	else if (argc > 1)
	{
		mshell.line = ft_strdup(argv[1]);
		if (!is_valid_quotes(mshell.line, &mshell.last_exit_status))
		{
			free_mshell(&mshell);
			return (1);
		}
		if (parse(&mshell))
		{
			free_mshell(&mshell);
			return (1);
		}
		exec(&mshell);
		mshell.last_exit_status = 0;
	}
	else
	{
		(void)argv;
		while (42)
		{
			mshell.line = readline("\033[1;34mminishell\033[0m\033[1;31m$\033[0m ");
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
			exec(&mshell);
		}
	}
	free_mshell(&mshell);
	return 0;
}