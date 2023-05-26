/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_other.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 21:22:41 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/21 01:11:38 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	u_backslash(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i])
	{
		if (arg[i] == '/')
			return (1);
	}
	return (0);
}

// int	u_isdots(char *arg)
// {
// 	int	i;
// 	int	count;

// 	count = 0;
// 	i = -1;
// 	while (arg[++i])
// 	{
// 		if (count == 2)
// 			return (count);
// 		if (arg[i] == '.')
// 			count++;
// 	}
// 	return (count);
// }

void	u_cmd_check(t_command *head)
{
	DIR	*dir;

	if (u_backslash(head->command))
	{
		dir = opendir(head->command);
		if (dir)
		{
			closedir(dir);
			u_errexit(head->command, ": Is a directory", 126);
		}
		else if (access(head->path, X_OK)
			&& access(head->path, F_OK))
			u_errexit(head->command, ": No such file or directory", 127);
	}
	if (access(head->path, X_OK) && !access(head->path, F_OK))
		u_errexit(head->command, ": Permission denied", 126);
	else if (!head->path)
		u_errexit(head->command, ": command not found", 127);
}

void	u_other_child(t_data *data)
{
	int	fd[2];

	signal(SIGQUIT, SIG_DFL);
	u_cmd_check((*data->head_of_commands));
	env_to_array(data);
	u_redir(data, &fd[0], &fd[1]);
	u_close_curr_cmd(data);
	if (execve((*data->head_of_commands)->path,
			(*data->head_of_commands)->arguments, data->env) == -1)
	{
		strerror(errno);
		exit(127);
	}
}

int	m_other(t_data *data)
{	
	data->exec->one_cmd_pid = fork();
	if (!data->exec->one_cmd_pid)
		u_other_child(data);
	u_wait(data, 0);
	return (0);
}
