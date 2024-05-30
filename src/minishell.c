/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/30 16:59:03 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_minishell mshell = {0};

	(void)argc;
	(void)argv;
	mshell.env = envp;
	// mshell.line = ft_strdup(argv[1]);
	// mshell.env = envp;
	// parse(&mshell);
	// exec(&mshell);
	// free(mshell.line);
	while (42)
	{
		mshell.line = readline("Tkt ca marche regarde -> ");
		if (!mshell.line)
			break ;
		if (*mshell.line)
			add_history(mshell.line);
		parse(&mshell);
		exec(&mshell);
		free(mshell.line);
	}
}
