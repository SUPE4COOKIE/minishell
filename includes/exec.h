/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:57:22 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/13 11:05:04 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# define TMP_FILE_PREFIX "/tmp/minishell_hdoc_"
# define URANDOM_PATH "/dev/urandom"
# define RANDOM_BYTES 8

typedef struct	s_redir_args
{
	t_cmd	*cmd;
	t_minishell	*mshell;
	int	old[2];
	int	new[2];
	int	i;
}				t_redir_args;

void	close_and_cpy(int old[2], int new[2], int i);
int		pipe_command(t_cmd *cmd, int new[2]);
void	handle_signal_process(void);
void next_command(t_cmd **cmd, int *i);
int		exec(t_minishell *mshell);
bool	is_builtin(char *cmd);
int		replace_hdoc(t_cmd *cmd, int old[2], int new[2]);
int		exec_builtin(t_minishell *mshell, t_cmd *cmd);
int		handle_file_redirection(t_minishell *mshell, t_cmd *cmd, \
		int old[2], int new[2]);
int		handle_hdoc(t_cmd *cmd, int old[2], int new[2], char **tmp_filename);
void	ft_close(int old[2], int new[2]);
int		init_exec(int old[2], int new[2], t_minishell *mshell);
void	close_old(int i, int old[2]);
int		fork_exec(t_minishell *mshell, int old[2], int new[2], int i);
int		dup_cmd(int i, t_cmd *cmd, int old[2], int new[2]);
int		exec_cmd(t_minishell *mshell, t_cmd *cmd);
int		generate_unique_filename(char *buffer, size_t length);
void	signal_exec(int signal);
void	signal_here_doc(int signal);
void	signal_new_line(int sig);
int		handle_red_out(t_cmd *cmd, t_minishell *mshell);
void	handle_append_out(t_cmd *cmd, int old[2], int new[2], \
		t_minishell *mshell);
void	handle_red_in(t_cmd *cmd, int old[2], int new[2], t_minishell *mshell);
int		check_infiles(t_cmd *cmd, t_minishell *mshell);
int		process_infile(t_cmd *cmd, t_minishell *mshell, int i);
int		check_outfiles(t_cmd *cmd, t_minishell *mshell, int i);
int		process_outfile(t_cmd *cmd, t_minishell *mshell, int i);
void	handle_file_redirections(t_minishell *mshell, t_cmd *cmd, \
		int old[2], int new[2]);
<<<<<<< HEAD
void	handle_outfiles(t_redir_args *args);
void	handle_redirections(t_cmd *cmd, int old[2], int new[2], t_minishell *mshell);
=======
int		handle_outfiles(t_redir_args *args);
void	handle_redirections(t_cmd *cmd, int old[2], int new[2], \
		t_minishell *mshell);
>>>>>>> db08939 (finished)

#endif