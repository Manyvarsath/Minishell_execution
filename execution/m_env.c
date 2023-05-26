/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 16:51:45 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 22:45:00 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	u_print_var(t_exec *exec, char *var, char *value)
{
	ft_putstr_fd(var, exec->fd);
	ft_putchar_fd('=', exec->fd);
	ft_putstr_fd(value, exec->fd);
	ft_putchar_fd('\n', exec->fd);
}

//! shouldn't print unset variables
int	m_env(t_data *data)
{
	t_env	*temp;

	temp = (*data->head_env);
	while (temp)
	{
		if (temp->var && ft_strcmp(temp->value, ""))
			u_print_var(data->exec, temp->var, temp->value);
		temp = temp->next;
	}
	g_last_exit_code = 0;
	return (g_last_exit_code);
}
