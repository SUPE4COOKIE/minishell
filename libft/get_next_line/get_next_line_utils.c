/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 12:36:53 by sonamcrumie       #+#    #+#             */
/*   Updated: 2024/03/25 16:28:40 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line.h"

char	*ft_bzero_and_free_gnl(char *buff, char *line)
{
	ft_bzero_gnl(buff, 1);
	return (free(line), NULL);
}

void	ft_bzero_gnl(void *s, size_t n)
{
	char	*temp;
	size_t	i;

	temp = (char *)s;
	i = 0;
	while (i < n)
	{
		temp[i] = '\0';
		i++;
	}
}

size_t	ft_strlen_gnl(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\n')
			return (i + 1);
		i++;
	}
	return (i);
}

char	*ft_memcpy_gnl(char *dst, char *src)
{
	int	i;

	if (dst == NULL && src == NULL)
		return (NULL);
	i = 0;
	while (src[i] != 0)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
	return (dst);
}

char	*print_line_gnl(char *line, char *buff, int *check_buff, int fd)
{
	*check_buff = ft_check_buff_gnl(buff, 1);
	if (*check_buff < BUFFER_SIZE)
	{
		line = ft_strjoin_gnl(line, buff);
		if (!line)
			return (free(line), NULL);
	}
	else
	{
		while (*check_buff == BUFFER_SIZE)
		{
			line = ft_strjoin_gnl(line, buff);
			if (!line)
				return (free(line), NULL);
			buff[read(fd, buff, BUFFER_SIZE)] = 0;
			*check_buff = ft_check_buff_gnl(buff, 1);
		}
		line = ft_strjoin_gnl(line, buff);
		if (!line)
			return (free(line), NULL);
	}
	return (line);
}
