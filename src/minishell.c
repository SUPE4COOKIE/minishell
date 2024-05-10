/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sonamcrumiere <sonamcrumiere@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 03:19:56 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/05/09 13:55:48 by sonamcrumie      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

void parse(t_minishell *mshell, char **envp, int ac, char **av)
{
    if (ac < 1)
        exit(1);
	mshell->env = envp;
    t_cmd *head = NULL;
    t_cmd *prev = NULL;
    for (int i = 0; i < ac; i++) {
        t_cmd *new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
        if (!new_cmd)
            exit(1);
        new_cmd->cmd = ft_strdup(av[i]);
		if (av[i] != '|')
        	new_cmd->type = CMD;
		else
			new_cmd->type = PIP;
        new_cmd->pid = -1;
        new_cmd->infile = NULL;
        new_cmd->outfile = NULL;
        new_cmd->next = NULL;
        if (prev != NULL) {
            prev->next = new_cmd;
        }
        if (head == NULL) {
            head = new_cmd;
        }
        prev = new_cmd;
    }
    mshell->cmd = head;
}

int find_env_index(const char *var_name, char **envp) {
    if (var_name == NULL || envp == NULL) {
        return -1;
    }
    
    int index = 0;
    while (envp[index] != NULL) {
        if (ft_strncmp(envp[index], var_name, ft_strlen(var_name)) == 0) {
            return index;
        }
        index++;
    }
    
    return -1;
}

int main(int argc, char **argv, char **envp)
{
    t_minishell mshell = {0};
    int path_index;

    parse(&mshell, envp, argc - 1, argv + 1);
    path_index = find_env_index("PATH=", envp);
    if (path_index != -1)
        mshell.env = &envp[path_index];
	else
	{
		printf("PATH not found\n");
		exit(1);
	}
    exec(&mshell);
    return 0;
}

