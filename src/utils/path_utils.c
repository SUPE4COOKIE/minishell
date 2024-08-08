/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42lyon.fr >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 23:49:05 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/08/07 23:49:27 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_sysbin(t_minishell *mshell)
{
	mshell->path = ft_split(DEFAULT_PATH, ':');
	if (!mshell->path)
		return (1);
	return (0);
}