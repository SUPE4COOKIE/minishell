/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   name_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:21:40 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/22 14:26:56 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define TMP_FILE_PREFIX "/tmp/minishell_hdoc_"
#define URANDOM_PATH "/dev/urandom"
#define RANDOM_BYTES 8

static int	open_urandom(void)
{
	int	urandom_fd;

	urandom_fd = open(URANDOM_PATH, O_RDONLY);
	if (urandom_fd < 0)
	{
		return (1);
	}
	return (urandom_fd);
}

static int	read_random_bytes(int urandom_fd, unsigned char *random_bytes)
{
	if (read(urandom_fd, random_bytes, RANDOM_BYTES) != RANDOM_BYTES)
	{
		close(urandom_fd);
		return (-1);
	}
	close(urandom_fd);
	return (0);
}

static void	convert_to_hex(unsigned char *random_bytes, char *hex_string)
{
	size_t	i;
	char	*hex_chars;

	hex_chars = "0123456789abcdef";
	i = 0;
	while (i < RANDOM_BYTES)
	{
		hex_string[2 * i] = hex_chars[(random_bytes[i] >> 4) & 0xF];
		hex_string[2 * i + 1] = hex_chars[random_bytes[i] & 0xF];
		i++;
	}
	hex_string[2 * RANDOM_BYTES] = '\0';
}

int	generate_unique_filename(char *buffer, size_t length)
{
	int				urandom_fd;
	unsigned char	random_bytes[RANDOM_BYTES];
	char			hex_string[RANDOM_BYTES * 2 + 1];

	urandom_fd = open_urandom();
	if (urandom_fd < 0)
		return (1);
	if (read_random_bytes(urandom_fd, random_bytes) != 0)
		return (1);
	convert_to_hex(random_bytes, hex_string);
	ft_strlcpy(buffer, TMP_FILE_PREFIX, length);
	ft_strlcat(buffer, hex_string, length);
	return (0);
}
