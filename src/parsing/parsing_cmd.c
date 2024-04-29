/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:34:36 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/04/29 05:05:02 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include "../includes/minishell.h"

void	check_args(int ac)
{
	if (ac != 5)
	{
		write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37);
		exit(1);
	}
}

void	cmd_nullcheck(t_pipex *pipex, char **av)
{
	if (!pipex->cmd[0][0])
	{
		free(pipex->cmd[0]);
		pipex->cmd[0] = NULL;
		write(2, "pipex: ", 7);
		write(2, av[2], ft_strlen(av[2]));
		write(2, " command not found\n", 19);
	}
	if (!pipex->cmd[1][0])
	{
		free(pipex->cmd[1]);
		pipex->cmd[1] = NULL;
		write(2, "pipex: ", 7);
		write(2, av[3], ft_strlen(av[3]));
		write(2, " command not found\n", 19);
	}
}

void	save_cmds(t_pipex *pipex, char **av)
{
	size_t	i;

	i = 0;
	if (!av[2] || !av[3])
		exit_error(pipex);
	pipex->cmd[0] = malloc(sizeof(char) * ft_strlen(av[2]) + 1);
	pipex->cmd[1] = malloc(sizeof(char) * ft_strlen(av[3]) + 1);
	if (!pipex->cmd[0] || !pipex->cmd[1])
		exit_error(pipex);
	while (av[2][i] && av[2][i] != ' ')
	{
		pipex->cmd[0][i] = av[2][i];
		i++;
	}
	pipex->cmd[0][i] = 0;
	i = 0;
	while (av[3][i] && av[3][i] != ' ')
	{
		pipex->cmd[1][i] = av[3][i];
		i++;
	}
	pipex->cmd[1][i] = 0;
}

void	save_args(t_pipex *pipex, char **av)
{
	pipex->args[0] = ft_split_args(av[2], ' ');
	if (!pipex->args[0])
		exit_error(pipex);
	pipex->args[1] = ft_split_args(av[3], ' ');
	if (!pipex->args[1])
		exit_error(pipex);
}