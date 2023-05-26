/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 10:55:42 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 21:39:11 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	u_num_cmds(t_data *data)
{
	t_command	*tmp;
	int			num;

	num = 0;
	tmp = (*data->head_of_commands);
	while (tmp)
	{
		num++;
		tmp = tmp->next;
	}
	return (num);
}

void	u_exec_init(t_data *data)
{
	data->exec->i = -1;
	data->exec->num_cmds = u_num_cmds(data);
	data->exec->num_pipes = data->exec->num_cmds - 1;
	if (data->exec->num_pipes > 0)
	{
		data->exec->pfd = (int **)malloc(sizeof(int *) * data->exec->num_pipes);
		while (++data->exec->i < data->exec->num_pipes)
		{
			data->exec->pfd[data->exec->i] = (int *)malloc(sizeof(int) * 2);
			if (pipe(data->exec->pfd[data->exec->i]) < 0)
				ft_putstr_fd("Pipe error", 2);
		}
		data->exec->pid = (pid_t *)malloc(sizeof(pid_t) * data->exec->num_cmds);
	}
}

void	free_pfd(t_data *data)
{
	data->exec->i = -1;
	while (++data->exec->i < data->exec->num_pipes)
		free(data->exec->pfd[data->exec->i]);
	free(data->exec->pfd);
}

void	execution(t_data *data, t_command **head)
{
	data->exec = malloc(sizeof(t_exec));
	u_exec_init(data);
	if (data->exec->num_cmds == 1 && (*data->head_of_commands)->command != NULL)
	{
		exe_uwu(data, 0);
		u_close_curr_cmd(data);
	}
	else if (data->exec->num_cmds > 1)
	{
		pipe_n_redir(data);
		free_pfd(data);
		free(data->exec->pid);
	}
	free(data->exec);
}
