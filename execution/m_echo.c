/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_echo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 04:28:58 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 22:44:22 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	m_echo_option(char **arg, int *option)
{
	int	i;
	int	j;

	i = 1;
	while (arg[i])
	{
		j = 0;
		if (arg[i][j] == '-')
		{
			while (arg[i][++j] == 'n')
				;
			if (arg[i][j] && arg[i][j] != 'n')
				return (i);
			(*option)++;
		}
		else
			return (i);
		i++;
	}
	return (i);
}

int	m_echo(t_data *data)
{
	int	option;
	int	i;

	option = 0;
	i = m_echo_option((*data->head_of_commands)->arguments, &option);
	while ((*data->head_of_commands)->arguments[i])
	{
		ft_putstr_fd((*data->head_of_commands)->arguments[i++], data->exec->fd);
		if ((*data->head_of_commands)->arguments[i])
			ft_putchar_fd(' ', data->exec->fd);
	}
	if (!option)
		ft_putchar_fd('\n', data->exec->fd);
	return (0);
}
