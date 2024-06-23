/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/23 22:23:42 by mwojtasi         ###   ########.fr       */
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
		//exec(&mshell);
		mshell.last_exit_status = 0;
		free_mshell(&mshell);
		return (0);
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
		//exec(&mshell);
		mshell.last_exit_status = 0;
		free_mshell(&mshell);
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
			//exec(&mshell);
			free_mshell(&mshell);
		}
	}
	return 0;
}
