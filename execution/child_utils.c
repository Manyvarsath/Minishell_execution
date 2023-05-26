/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 21:35:16 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 22:03:42 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	u_redir(t_data *data, int *f_in, int *f_out)
{
	*f_in = STDIN_FILENO;
	*f_out = STDOUT_FILENO;
	if (m_choice((*data->head_of_commands)->command) == 7)
	{
		if ((*data->head_of_commands)->in_fd != -1)
		{
			dup2((*data->head_of_commands)->in_fd, STDIN_FILENO);
			*f_in = (*data->head_of_commands)->in_fd;
		}
		if ((*data->head_of_commands)->out_fd != -1)
		{
			dup2((*data->head_of_commands)->out_fd, STDOUT_FILENO);
			*f_out = (*data->head_of_commands)->out_fd;
		}
	}
	else
	{
		u_redir_builtins(data);
		*f_out = data->exec->fd;
	}
}

void	u_close_curr_cmd(t_data *data)
{
	if ((*data->head_of_commands)->files)
	{
		while ((*(*data->head_of_commands)->files)
			&& (*(*data->head_of_commands)->files)->fd > 0)
		{
			close((*(*data->head_of_commands)->files)->fd);
			free(*(*data->head_of_commands)->files);
			(*(*data->head_of_commands)->files)
				= (*(*data->head_of_commands)->files)->next;
		}
	}
}

void	u_cleanup(t_data *data)
{
	data->exec->i = -1;
	while (++data->exec->i < data->exec->num_pipes)
	{
		close(data->exec->pfd[data->exec->i][0]);
		close(data->exec->pfd[data->exec->i][1]);
	}
}

int	u_wait(t_data *data, int is_mpipe)
{
	data->exec->i = -1;
	signal(SIGINT, SIG_IGN);
	if (is_mpipe)
	{
		while (++data->exec->i < data->exec->num_cmds)
			waitpid(data->exec->pid[data->exec->i], &g_last_exit_code, 0);
	}
	else
		waitpid(data->exec->one_cmd_pid, &g_last_exit_code, 0);
	if (WIFEXITED(g_last_exit_code))
		g_last_exit_code = WEXITSTATUS(g_last_exit_code);
	else if (WIFSIGNALED(g_last_exit_code))
	{
		if (WTERMSIG(g_last_exit_code) == SIGQUIT)
			ft_putendl_fd("Quit: 3", 2);
		g_last_exit_code = WTERMSIG(g_last_exit_code) + 128;
	}
	return (g_last_exit_code);
}
