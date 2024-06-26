/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/26 15:13:42 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_sig;

void signal_new_line(int sig)
{
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

void	signal_here_doc(int signal)
{
	g_sig = signal;
	rl_done = 1;
}

void	signal_exec(int signal)
{
	g_sig = signal;
}

int readline_event_hook(void)
{
	signal(SIGINT, signal_here_doc);
	rl_done = 1;
	return (0);
}

int main(int argc, char **argv, char **envp)
{
    t_minishell mshell = {0};

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
	save_path(&mshell, mshell.env); // TODO: protect
	mshell.last_exit_status = 0;
	mshell.last_pid = 0;
	g_sig = 0;
	mshell.in_heredoc = false;
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
			signal(SIGINT, signal_new_line);
			signal(SIGQUIT, signal_new_line);
			g_sig = 0;
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
			free(mshell.line);
			free_cmds(mshell.cmds);
		}
	}
	free_mshell(&mshell);
	return 0;
}
