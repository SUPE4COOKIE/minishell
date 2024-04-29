/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtasi <mwojtasi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 11:28:30 by mwojtasi          #+#    #+#             */
/*   Updated: 2024/04/29 04:30:26 by mwojtasi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

static t_list	*lstnew_del(void *content, void (*del)(void *))
{
	t_list	*new;

	new = ft_lstnew(content);
	if (!new)
	{
		del(content);
		return (NULL);
	}
	return (new);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*result;
	t_list	*current;
	t_list	*new;

	if (!lst || !f)
		return (NULL);
	current = lst;
	result = NULL;
	while (current)
	{
		new = lstnew_del(f(current->content), del);
		if (!new)
		{
			ft_lstclear(&result, del);
			return (NULL);
		}
		ft_lstadd_back(&result, new);
		current = current->next;
	}
	return (result);
}
