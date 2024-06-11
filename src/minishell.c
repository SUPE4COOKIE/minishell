/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/11 20:13:49 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
	(void)argc;
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
	mshell.env = envp;
	mshell.last_exit_status = 0;
	if (!PROMPT_ENABLED)
	{
		mshell.line = ft_strdup(argv[1]);
		parse(&mshell);
		//exec(&mshell);
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
			if (!mshell.line)
				break ;
			if (*mshell.line)
				add_history(mshell.line);
			parse(&mshell);
			free(mshell.line);
		}
	}
}
