/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:35:17 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/06/14 15:38:00 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static inline int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token_type	get_lexer_type(char *line)
{
	if (line[0] == '"' )
		return (T_D_QUOTED_WORD);
	else if (line[0] == '\'')
		return (T_S_QUOTED_WORD);
	else if (line[0] == '|')
		return (T_PIPE);
	else if (line[0] == '<')
	{
		if (line[1] == '<')
			return (T_HERE_DOC);
		else
			return (T_REDIR_IN);
	}
	else if (line[0] == '>')
	{
		if (line[1] == '>')
			return (T_APPEND_OUT);
		else
			return (T_REDIR_OUT);
	}
	else
		return (T_WORD);
}

void append_new_lexer(t_lexer **lex, t_lexer **new)
{
	t_lexer *tmp;

	if (*lex == NULL)
	{
		*lex = *new;
		(*lex)->prev = NULL;
	}
	else
	{
		tmp = *lex;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = *new;
		(*new)->prev = tmp;
	}
}


int is_n_only_spaces(char *line, size_t size)
{
	size_t i;

	i = 0;
	while (i < size)
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' 
			&& line[i] != '\v' && line[i] != '\f' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

char *get_token_type(t_token_type type)
{
	if (type == T_WORD)
		return ("T_WORD");
	else if (type == T_PIPE)
		return ("T_PIPE");
	else if (type == T_REDIR_IN)
		return ("T_REDIR_IN");
	else if (type == T_REDIR_OUT)
		return ("T_REDIR_OUT");
	else if (type == T_APPEND_OUT)
		return ("T_APPEND_OUT");
	else if (type == T_D_QUOTED_WORD)
		return ("T_D_QUOTED_WORD");
	else if (type == T_S_QUOTED_WORD)
		return ("T_S_QUOTED_WORD");
	else if (type == T_HERE_DOC)
		return ("T_HERE_DOC");
	else
		return ("UNKNOWN");
}

void	print_lexer(t_lexer *lex)
{
	while (lex)
	{
		printf("value: %s\n", lex->value);
		printf("type: %s\n", get_token_type(lex->type));
		printf("space_after: %d\n\n", lex->space_after);
		lex = lex->next;
	}
}

int	split_word_lexer(t_lexer **lex, char *line, size_t size)
{
	size_t	i;
	int		error_code;

	i = 0;
	while (i < size)
	{
		if (ft_iswhitespace(line[i]))
		{
			if (i > 0)
			{
				error_code = new_lexer(lex, line, i); // TODO: check return
				if (error_code)
					return (error_code);
				if (!(*lex))
					return (0);
				get_last_lexer(*lex)->space_after = true;
			}
			line += i + 1;
			size -= i + 1;
			i = 0;
		}
		i++;
	}
	if (size > 0)
	{
		new_lexer(lex, line, size);
		if (ft_iswhitespace(line[size - 1]))
			get_last_lexer(*lex)->space_after = true;
		else
			get_last_lexer(*lex)->space_after = false;
	}
	return (0);
}

int	new_lexer(t_lexer **lex, char *line, size_t size)
{
	//possible upgrade : return a t_lexer instead of int
	t_lexer	*new;
	char	*trim;
	
	if (!line || !line[0])
		return (0);
	if (is_n_only_spaces(line, size))
		return (0);
	new = malloc(sizeof(t_lexer));
	if (new == NULL)
		return (1); //TODO: free exit
	new->value = malloc(size + 1);
	if (new->value == NULL)
		return (1); //TODO: free exit
	//printf("%.*s\n", (int)size, line);
	if (line[0] == '"' || line[0] == '\'')
		ft_strlcpy(new->value, line + 1, size);
	else
	{
		ft_strlcpy(new->value, line, size + 1); //TODO: check return
		trim = new->value;
		new->value = ft_strtrim(new->value, " "); // TODO: check return
		free(trim);
	}
	new->type = get_lexer_type(line);
	append_new_lexer(lex, &new);
	new->next = NULL;
	new->space_after = false;
	// temp code
	//printf("value: %s\n", new->value);
	//printf("type: %s\n", get_token_type(new->type));
	return (0);
}


void	free_lexer(t_lexer *lex)
{
	t_lexer *temp;

	while (lex)
	{
		temp = lex;
		lex = lex->next;
		free(temp->value);
		free(temp);
	}
}

void add_operator(t_lexer **lex, char *line, size_t *end)
{
	size_t len;

	len = 0;
	while (line[len] && is_operator_char(line[len]) && line[0] == line[len] && len < 2)
	{
		len++;
		if (line[0] == '|')
			break ;
	}
	new_lexer(lex, line, len); // TODO: check return
	(*end) += len;
	if (ft_iswhitespace(line[len]))
		get_last_lexer(*lex)->space_after = true;
	else
		get_last_lexer(*lex)->space_after = false;
	//printf("space_after: %d\n\n", get_last_lexer(*lex)->space_after);
}

t_lexer	*get_last_lexer(t_lexer *lex)
{
	while (lex && lex->next)
		lex = lex->next;
	return (lex);
}

t_lexer	*delete_lexer(t_lexer **lex, t_lexer *to_delete)
{
	t_lexer	*tmp;
	t_lexer	*to_return;

	tmp = *lex;
	to_return = tmp->next;
	if (tmp == to_delete)
	{
		*lex = tmp->next;
		free(tmp->value);
		free(tmp);
		return (to_return);
	}
	while (tmp && tmp != to_delete)
    tmp = tmp->next;
	if (tmp && tmp == to_delete)
	{
    	if (tmp->next)
    	    to_return = tmp->next;
    	if (tmp->prev)
    	    tmp->prev->next = tmp->next;
    	if (tmp->next)
    	    tmp->next->prev = tmp->prev;
    	free(tmp->value);
    	free(tmp);
	}
	return (to_return);
}

int	add_quoted_word(t_lexer **lex, char *line, size_t *end)
{
	size_t	len;
	t_lexer *tmp;
	char	quote;

	quote = line[0];
	len = 1;
	while (line[len] && line[len] != quote)
		len++;
	if (len > 1)
		new_lexer(lex, line, len); // TODO: check return
	if (line[len] == quote)
		len++;
	(*end) += len;
	tmp = get_last_lexer(*lex);
	if (ft_iswhitespace(line[len]) && tmp)
		tmp->space_after = true;
	else if (tmp && !tmp->space_after)
		tmp->space_after = false;
	return (0);
}

int	lexer_director(t_lexer **lex, size_t *end, size_t *start, char *line)
{
	while (line[*end])
	{
		if (line[*end] == '"' || line[*end] == '\'')
		{
			if (*end > *start)
			{
				if (get_lexer_type(line + *start) == T_WORD)
				{
					if (split_word_lexer(lex, line + *start, *end - *start))// TODO: check return
						return (1);
				}
				else
				{
					if (new_lexer(lex, line + *start, *end - *start)) // TODO: check return
						return (1);
					if (ft_iswhitespace(line[*end]))
						get_last_lexer(*lex)->space_after = true;
					else
						get_last_lexer(*lex)->space_after = false;
					//printf("space_after: %d\n\n", get_last_lexer(*lex)->space_after);
				}
			}
			*start = *end;
			add_quoted_word(lex, line + *start, end); // TODO: check return
			*start = *end;
			continue; // not retested might cause problems but avoid invalid read
		}
		if (is_operator_char(line[*end]))
		{
			if (*end > *start)
			{
				if (get_lexer_type(line + *start) == T_WORD)
				{
					if (split_word_lexer(lex, line + *start, *end - *start)) // TODO: check return
						return (1);
				}
				else
				{
					if (new_lexer(lex, line + *start, *end - *start))// TODO: check return
						return (1);
					if (ft_iswhitespace(line[*end]))
						get_last_lexer(*lex)->space_after = true;
					else
						get_last_lexer(*lex)->space_after = false;
					//printf("space_after: %d\n\n", get_last_lexer(*lex)->space_after);
				}
			}
			*start = *end;
			while (line[*end] && is_operator_char(line[*end]))
			{
				add_operator(lex, line + *start, end); // TODO: check return
				*start = *end;
			}
			continue;
		}
		(*end)++;
	}
	return (0);
}

t_lexer	*lexer(char *line)
{
	t_lexer *lex;
	size_t	end;
	size_t	start;

	end = 0;
	start = 0;
	lex = NULL;
	if (lexer_director(&lex, &end, &start, line))
	{
		free_lexer(lex);
		return (NULL);
	}
	if (end > start)
	{
		if (get_lexer_type(line + start) == T_WORD)
			split_word_lexer(&lex, line + start, end - start); // TODO: check return
		else
		{
			new_lexer(&lex, line + start, end - start); // TODO: check return
			get_last_lexer(lex)->space_after = false;
			//printf("space_after: %d\n\n", get_last_lexer(lex)->space_after);
		}
	}
	return (lex);
}