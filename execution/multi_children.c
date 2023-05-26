/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_children.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 21:31:38 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/21 01:28:06 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	u_first_child(t_data *data)
{
	int	fd[2];

	env_to_array(data);
	u_redir(data, &fd[0], &fd[1]);
	if ((*data->head_of_commands)->out_fd == -1)
		dup2(data->exec->pfd[0][1], fd[1]);
	if (exe_uwu(data, 1) != -1)
	{
		u_cleanup(data);
		u_close_curr_cmd(data);
		exit(g_last_exit_code);
	}
	dup2(data->exec->pfd[0][1], fd[1]);
	u_cleanup(data);
	u_close_curr_cmd(data);
	u_cmd_check((*data->head_of_commands));
	if (execve((*data->head_of_commands)->path,
			(*data->head_of_commands)->arguments, data->env))
		u_errexit((*data->head_of_commands)->command, "Command not found", 127);
}

void	u_last_child(t_data *data)
{
	int	fd[2];

	env_to_array(data);
	u_redir(data, &fd[0], &fd[1]);
	if ((*data->head_of_commands)->out_fd == -1)
		dup2(data->exec->pfd[data->exec->num_pipes - 1][0], fd[0]);
	if (exe_uwu(data, 1) != -1)
	{
		u_cleanup(data);
		u_close_curr_cmd(data);
		exit(g_last_exit_code);
	}
	dup2(data->exec->pfd[data->exec->num_pipes - 1][0], fd[0]);
	u_cleanup(data);
	u_close_curr_cmd(data);
	u_cmd_check((*data->head_of_commands));
	if (execve((*data->head_of_commands)->path,
			(*data->head_of_commands)->arguments, data->env) == -1)
		u_errexit((*data->head_of_commands)->command, "Command not found", 127);
}

void	u_mid_child(t_data *data, int index)
{
	int	fd[2];

	env_to_array(data);
	u_redir(data, &fd[0], &fd[1]);
	if ((*data->head_of_commands)->out_fd == -1)
	{
		dup2(data->exec->pfd[index - 1][0], fd[0]);
		dup2(data->exec->pfd[index][1], fd[1]);
	}
	if (exe_uwu(data, 1) != -1)
	{
		u_cleanup(data);
		u_close_curr_cmd(data);
		exit(g_last_exit_code);
	}
	dup2(data->exec->pfd[index - 1][0], fd[0]);
	dup2(data->exec->pfd[index][1], fd[1]);
	u_cleanup(data);
	u_close_curr_cmd(data);
	u_cmd_check((*data->head_of_commands));
	if (execve((*data->head_of_commands)->path,
			(*data->head_of_commands)->arguments, data->env))
		u_errexit((*data->head_of_commands)->command, "Command not found", 127);
}

void	u_children(t_data *data)
{
	signal(SIGQUIT, SIG_DFL);
	if (data->exec->i == 0)
		u_first_child(data);
	else if (data->exec->i == data->exec->num_cmds - 1)
		u_last_child(data);
	else
		u_mid_child(data, data->exec->i);
}

void	pipe_n_redir(t_data *data)
{
	t_command	**temp_head;

	temp_head = data->head_of_commands;
	data->exec->i = -1;
	while (++data->exec->i < data->exec->num_cmds
		&& (*data->head_of_commands)->command)
	{
		data->exec->pid[data->exec->i] = fork();
		if (data->exec->pid[data->exec->i] < 0)
		{
			error_message("Fork");
			break ;
		}
		else if (data->exec->pid[data->exec->i] == 0)
			u_children(data);
		u_close_curr_cmd(data);
		data->head_of_commands = &((*data->head_of_commands)->next);
	}
	data->head_of_commands = temp_head;
	u_cleanup(data);
	u_wait(data, 1);
}
