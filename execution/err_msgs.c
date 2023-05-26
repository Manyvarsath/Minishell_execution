/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_msgs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 21:33:30 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 23:00:30 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_message(char *err)
{
	perror(err);
}

int	err_export(char *type, char *new)
{
	ft_putstr_fd(type, 2);
	ft_putstr_fd(new, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_last_exit_code = 1;
	return (g_last_exit_code);
}

int	u_errexit(char *err, char *type, int num)
{
	error_msg(err, type);
	exit(num);
}
