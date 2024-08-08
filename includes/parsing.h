/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 17:01:12 by scrumier          #+#    #+#             */
/*   Updated: 2024/08/08 00:28:20 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

struct s_lexer
{
	char			*value;
	bool			space_after;
	t_token_type	type;
	t_lexer			*next;
	t_lexer			*prev;
};

int		parse(t_minishell *mshell);
int		is_in_quote(char c, char *in_quote);
t_lexer	*lexer(char *line);
bool	validate(t_lexer *lex, int *exit_code);
int		new_lexer(t_lexer **lex, char *line, size_t size);
int		save_path(t_minishell *mshell, char **env);
t_cmd	*lexer_to_cmd(t_lexer *lex, char **path, int *exit_status);
t_lexer	*get_last_lexer(t_lexer *lex);
int		get_cmd_path(t_cmd **cmd, char **path, int *exit_status);
t_cmd	*delete_cmd(t_cmd **cmd, t_cmd *to_delete);
t_lexer	*delete_lexer(t_lexer **lex, t_lexer *to_delete);
int		allocate_env(t_minishell *mshell, char **envp);
void	free_env_path(t_minishell *mshell);
void	free_lexer(t_lexer *lex);
bool	is_valid_quotes(char *line, int *status_code);
void	free_cmds(t_cmd *cmd);
int		parse_path(t_minishell *mshell, char *env);
t_lexer	*lexer_replacer(t_lexer *lex, char *value, t_lexer **origin);
void	skip_var(char **var_value);
void	copy_value(char **result, char **value, size_t *iter, size_t *i);
void	copy_var(char **result, char **var_value, size_t *i);
char	*var_replacer(t_lexer *var, char *value, size_t *iter);
char	*var_finder(char *var, char **envp);
char	*get_name(char *str);
bool	contain_spaced_words(char *str);
int		get_var_value(t_lexer **lex, char **var, size_t *i, char **envp);
int		existing_var(t_lexer **lex, size_t *i, char **var);
int		inexistant_var(t_lexer **lex, size_t *i, char **var);
int		var_replace_manager(t_lexer **lex, size_t *i, char **var);
int		var_manager(t_lexer **lex, size_t *i, char **var, int last_exit_status);
t_lexer	*get_first_lexer(t_lexer *lex);
int		lexer_iter(t_lexer **lex, size_t *i, char **envp, int last_exit_status);
int		expand(t_lexer **lex, char **envp, int last_exit_status);
bool	is_operator_char(char c);
t_token_type	get_lexer_type(char *line);
void	append_new_lexer(t_lexer **lex, t_lexer **new);
int		is_n_only_spaces(char *line, size_t size);
char	*get_token_type(t_token_type type);
int	split_remaining(t_lexer **lex, char *line, size_t size);
size_t	skip_whitespaces(char *line, size_t size);
int	split_word_lexer(t_lexer **lex, char *line, size_t size);
int	handle_new_lexer_value(t_lexer *new, char *line, size_t size);
int	new_lexer(t_lexer **lex, char *line, size_t size);
void	free_lexer(t_lexer *lex);
int	add_operator(t_lexer **lex, char *line, size_t *end);
t_lexer	*get_last_lexer(t_lexer *lex);
t_lexer	*delete_lexer(t_lexer **lex, t_lexer *to_delete);
int	add_quoted_word(t_lexer **lex, char *line, size_t *end);
int	process_word(t_lexer **lex, char *line, size_t start, size_t end);
int handle_quotes(t_lexer **lex, size_t *start, size_t *end, char *line);
int handle_operator(t_lexer **lex, size_t *start, size_t *end, char *line);
int lexer_director(t_lexer **lex, size_t *end, size_t *start, char *line);
int	finalize_lexer(t_lexer **lex, char *line, size_t start, size_t end);
int	dir_error(t_cmd **cmd, int *exit_status);
bool	is_point(t_cmd **cmd);
int	error_permissions(t_cmd **cmd, int *exit_status);
int	error_command_not_found(t_cmd **cmd, int *exit_status);
int	set_sysbin(t_minishell *mshell);
void free_str_array(char **array);
void free_cmd(t_cmd *cmd);
void free_cmds(t_cmd *cmd);
void redir_before_init(size_t *i, size_t *redir_in, size_t *redir_out, bool *reference_found);
void	is_redir_reference(char *redir, char *reference, bool *reference_found);
bool	is_redir_before(t_cmd *cmd, char **redir, char **reference);
t_cmd	*new_cmd(char **args);
int		append_cmd(t_cmd **cmd, t_cmd *new);
t_cmd	*get_last_cmd(t_cmd *cmd);
size_t	get_cmd_size(t_lexer *lex);
size_t	get_word_num(t_lexer *lex);
int append_new_typechain(t_cmd_type **type_chain, t_cmd_type type);
int	append_type(t_cmd_type **type_chain, t_cmd_type type);
int handle_redir_in(t_cmd *cmd, t_lexer **lex);
int handle_redir_out(t_cmd *cmd, t_lexer **lex, int type);
int handle_here_doc(t_cmd *cmd, t_lexer **lex);
int append_redir(t_cmd *cmd, t_lexer **lex);
bool	is_nospace_addable(t_token_type type);
int	nospace_add(char **args, t_lexer **lex);
int	allocate_args_cmd(t_lexer *lex, char ***args, char ***args_start, t_cmd **last_cmd);
bool is_redir(t_token_type type);
int	handle_args(char ***args, t_lexer **lex);
int	set_last_cmd(t_cmd *last_cmd, char **args_start);
int	append_cmds(t_cmd **cmd, t_lexer **lex);
t_cmd	*delete_cmd(t_cmd **cmd, t_cmd *to_delete);
int	resolve_cmd_path(t_cmd **cmd, char **path, int *exit_status);
t_cmd	*lexer_to_cmd(t_lexer *lex, char **path, int *exit_status);
#endif