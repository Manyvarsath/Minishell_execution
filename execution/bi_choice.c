/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_choice.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 08:13:12 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 22:01:17 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//dispatch table
typedef int	(*t_builtins)(t_data *data);

int	m_choice(char *choice)
{
	char	*choices[8];
	int		i;

	i = -1;
	choices[0] = "echo";
	choices[1] = "cd";
	choices[2] = "pwd";
	choices[3] = "export";
	choices[4] = "unset";
	choices[5] = "env";
	choices[6] = "exit";
	choices[7] = NULL;
	while (choices[++i])
	{
		if (!ft_strncmp(choices[i], choice, ft_strlen(choices[i]) + 1))
			return (i);
	}
	return (7);
}

int	m_out(t_data *data)
{
	(void)data;
	return (-1);
}

void	u_redir_builtins(t_data *data)
{
	if ((*data->head_of_commands)->out_fd != -1)
		data->exec->fd = (*data->head_of_commands)->out_fd;
	else
		data->exec->fd = STDOUT_FILENO;
}

int	exe_uwu(t_data *data, int is_child)
{
	t_builtins	builtins_choice[8];
	int			fd[2];

	builtins_choice[0] = m_echo;
	builtins_choice[1] = m_cd;
	builtins_choice[2] = m_pwd;
	builtins_choice[3] = m_export;
	builtins_choice[4] = m_unset;
	builtins_choice[5] = m_env;
	builtins_choice[6] = m_exit;
	if (m_choice((*data->head_of_commands)->command) < 7 && !is_child)
		u_redir_builtins(data);
	if (!is_child)
		builtins_choice[7] = m_other;
	else
		builtins_choice[7] = m_out;
	return (builtins_choice
		[m_choice((*data->head_of_commands)->command)](data));
}
