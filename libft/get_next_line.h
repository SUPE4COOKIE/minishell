/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 12:36:56 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/03/25 18:28:42 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# include <unistd.h>
# include <stdlib.h>

char	*ft_strdup_gnl(char *s);
char	*check_input_and_init_line_gnl(int fd);
void	ft_bzero_gnl(void *s, size_t n);
size_t	ft_strlen_gnl(char *s);
char	*ft_memcpy_gnl(char *dst, char *src);
char	*ft_strjoin_gnl(char *s1, char *s2);
int		ft_check_buff_gnl(char *buff, int choice);
char	*get_next_line(int fd);
char	*print_line_gnl(char *line, char *buff, int *check_buff, int fd);
char	*ft_bzero_and_free_gnl(char *buff, char *line);

#endif