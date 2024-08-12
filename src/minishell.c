/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/12 14:50:27 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig;

void	start_mshell(t_minishell *mshell)
{
	mshell->line = NULL;
	signal(SIGINT, signal_new_line);
	signal(SIGQUIT, signal_new_line);
	rl_event_hook = event;
	mshell->line = readline("minishell$> ");
	signal(SIGINT, signal_exec);
}

int	handle_mshell(t_minishell *mshell)
{
	while (42)
	{
		start_mshell(mshell);
		if (!mshell->line || g_sig == SIGQUIT)
			return (free_env_path(mshell), printf("exit\n"), \
					mshell->last_exit_status);
		if (!is_valid_quotes(mshell->line, &mshell->last_exit_status))
		{
			free_null(mshell->line);
			continue ;
		}
		if (is_n_only_spaces(mshell->line, ft_strlen(mshell->line)) == 1)
		{
			free_null(mshell->line);
			continue ;
		}
		if (*mshell->line)
			add_history(mshell->line);
		if (parse(mshell))
			continue ;
		if (exec(mshell) != 0)
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
	if (save_path(&mshell, mshell.env))
		return (free_tab(mshell.env), 1);
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		handle_dash_c(&mshell, argc, argv);
		return (mshell.last_exit_status);
	}
	else
		handle_mshell(&mshell);
	mshell.line = NULL;
	return (0);
}
