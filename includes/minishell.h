/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 17:18:24 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/07/22 14:17:41 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <errno.h>
# include <stdint.h>
# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif
# define PRINT_CAT false
# define DEBUG false
# define DEFAULT_PATH \
		"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
# include "struct.h"
# include "parsing.h"
# include "utils.h"
# include "exec.h"
# include "builtins.h"

extern volatile sig_atomic_t	g_sig;
void	print_cat(void);
int		event(void);
int		handle_dash_c(t_minishell *mshell, int argc, char **argv);

#endif