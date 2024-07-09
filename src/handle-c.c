/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle-c.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:18:54 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/09 12:49:47 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_dash_c(t_minishell *mshell, int argc, char **argv)
{
    if (argc > 1 && strcmp(argv[1], "-c") == 0)
    {
        mshell->line = ft_strdup(argv[2]);
        //mshell->line[ft_strlen(mshell->line - 1)] = 0;
        if (!is_valid_quotes(mshell->line, &mshell->last_exit_status))
        {
            free_mshell(mshell);
            return (1);
        }
        if (parse(mshell))
        {
            free_mshell(mshell);
            return (1);
        }
        exec(mshell);
    }
    return (0);
}
