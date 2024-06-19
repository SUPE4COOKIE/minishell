/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/19 19:35:25 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	mshell.last_exit_status = 0;
	if (argc > 1 && strcmp(argv[1], "-c") == 0)
	{
		mshell.line = ft_strdup(argv[2]);
		mshell.line[ft_strlen(mshell.line) - 1] = 0;
		parse(&mshell);
		exec(&mshell);
		free(mshell.line);
		mshell.last_exit_status = 0;
		return (0);
	}
	else if (argc > 1)
	{
		mshell.line = ft_strdup(argv[1]);
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
			mshell.line = readline("\033[1;34mminishell\033[0m\033[1;31m$\033[0m ");
			if (!mshell.line)
				break ;
			if (is_n_only_spaces(mshell.line, ft_strlen(mshell.line)) == 1)
			{
				free(mshell.line);
				continue ;
			}
			if (*mshell.line)
				add_history(mshell.line);
			parse(&mshell);
			exec(&mshell);
			free(mshell.line);
		}
	}
	return 0;
}
