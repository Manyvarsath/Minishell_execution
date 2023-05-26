/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/30 00:53:28 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 22:46:17 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	num_args(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
		;
	return (i);
}

static void	u_exit_err(int which, char *err)
{
	ft_putstr_fd("exit\nminishell: exit: ", STDERR_FILENO);
	if (which == 0)
		ft_putstr_fd("too many arguments\n", STDERR_FILENO);
	else if (which == 1)
	{
		ft_putstr_fd(err, 2);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	}
}

static int	is_space(char c)
{
	if (c == '\t' || c == '\v' || c == '\f' || c == '\r'
		|| c == '\n' || c == ' ')
		return (1);
	return (0);
}

static long long	ll_atoi(char *s, int *flag)
{
	int					sign;
	int					i;
	unsigned long long	nb;

	i = 0;
	sign = 1;
	nb = 0;
	while (is_space(s[i]))
		i++;
	if (s[i] && (s[i] == '-' || s[i] == '+') && (s[i++] == '-'))
		sign *= -1;
	while (s[i] >= '0' && s[i] <= '9')
	{
		nb = (nb * 10) + (s[i] - '0');
		i++;
	}
	if ((s[i] && (s[i] < '0' || s[i] > '9'))
		|| (nb > LLONG_MIN && sign == -1) || (nb > LLONG_MAX && sign == 1))
		*flag = 1;
	return (nb * sign);
}

int	m_exit(t_data *data)
{
	data->exec->i = 0;
	g_last_exit_code = 0;
	if (num_args((*data->head_of_commands)->arguments) > 2)
	{
		u_exit_err(0, "");
		g_last_exit_code = 1;
		return (g_last_exit_code);
	}
	else if (num_args((*data->head_of_commands)->arguments) == 2)
	{
		g_last_exit_code = (ll_atoi((*data->head_of_commands)->arguments[1],
					&data->exec->i) % 256);
		if (data->exec->i == 1)
		{
			u_exit_err(1, (*data->head_of_commands)->arguments[1]);
			g_last_exit_code = 255;
			exit (g_last_exit_code);
		}
		if (g_last_exit_code < 0)
			g_last_exit_code += 256;
	}
	free_stuff(data);
	exit (g_last_exit_code);
}
