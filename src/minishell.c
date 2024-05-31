/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/31 18:08:48 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_minishell mshell = {0};

	(void)argc;
	mshell.env = envp;
	mshell.line = ft_strdup(argv[1]);
	// maybe cpy in heap
	parse(&mshell);
	//exec(&mshell);
	free(mshell.line);
	//while (42)
	//{
	//	mshell.line = readline("minishell$ ");
	//	if (!mshell.line)
	//		break ;
	//	if (*mshell.line)
	//		add_history(mshell.line);
	//	parse(&mshell);
	//	free(mshell.line);
	//}
}
