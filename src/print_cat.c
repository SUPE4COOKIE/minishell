/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scrumier <scrumier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 10:18:57 by scrumier          #+#    #+#             */
/*   Updated: 2024/07/10 05:20:06 by scrumier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void print_cat(void)
{
    if (PRINT_CAT)
    {
        ft_printf("    ▓▓▓▒▒▒▒░░░▒▒▒████▒▒▒▒▒▒░░   ░▒▒▒▒▒░      \n");
        ft_printf("     ██▓▒▒░░▒▒▓██████▒░░░░░▒▒░ ░░░▒▒         \n");
        ft_printf("        ░░░░▒░░▒██████▒░░░   ░▒░░░░          \n");
        ft_printf("        ▒░░░░   ▒██████▒▒  ░ ▒▒░░░░          \n");
        ft_printf("         ░░▒░    ██████▒░░   ▓░░░░           \n");
        ft_printf("         ▒░░▒▒▒▒▒███████▒▒▓▒▒░  ░            \n");
        ft_printf("         ▒░░░▓████████████▓▓░  ░░            \n");
        ft_printf("          ▒▒▓████████████████▒ ░             \n");
        ft_printf("          ███████████████████▓▓░             \n");
        ft_printf("        ██▓▒▒██████▓▓█▓██████████            \n");
        ft_printf("        ▓▒▒▓▓████████▓████████████           \n");
        ft_printf("       ▒▒▒▒▒▒███████▓▒▓██████████            \n");
        ft_printf("       ▒▒░▒░▒▓██████████████████             \n");
        ft_printf("      ░ ░░░░░▓██████████████████             \n");
        ft_printf("     ░░   ░░░░▓█████████████████             \n");
        ft_printf("      ░    ░░░▒████████████████              \n");
        ft_printf("      ░░░░   ░▒███████████████               \n");
        ft_printf("              ░▓██████████████               \n");
        ft_printf("               ▒██████████████               \n");
        ft_printf("               ░░░▒██      █████             \n");
        ft_printf("                ░ ░▒▒       ██████           \n");
        ft_printf("                 ░░ ░▒                       \n");
    }
}
