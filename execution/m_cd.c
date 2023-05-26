/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 19:38:07 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 22:43:38 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*u_check_home(t_env **env)
{
	t_env	*temp;

	temp = *env;
	while (temp)
	{
		if (!ft_strncmp(temp->var, "HOME", 5) && temp->value)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

static int	u_cd_home(t_data *data)
{
	if (!u_check_home(data->head_env) || chdir(u_check_home(data->head_env)))
	{
		error_msg("cd : HOME: is undefined", NULL);
		return (1);
	}
	else if (!chdir(u_check_home(data->head_env)))
		u_env_switch(data, u_check_home(data->head_env));
	return (0);
}

static int	u_cd_minus(t_data *data)
{
	if (!data->old_working_dir || chdir(data->old_working_dir))
	{
		error_msg("cd: OLDPWD: not set", NULL);
		return (1);
	}
	if (!chdir(data->old_working_dir))
	{
		u_env_switch(data, data->old_working_dir);
		ft_putendl_fd(data->working_dir, data->exec->fd);
	}
	return (0);
}

int	m_cd(t_data *data)
{
	char	**arguments;

	arguments = (*data->head_of_commands)->arguments;
	g_last_exit_code = 0;
	if (arguments[1] && arguments[2])
	{
		error_msg("cd: too many arguments", NULL);
		g_last_exit_code = 1;
	}
	else if (arguments[1] && !arguments[2])
	{
		if (!ft_strncmp(arguments[1], ".", 2))
			;
		else if (!ft_strncmp(arguments[1], "-", 2))
			g_last_exit_code = u_cd_minus(data);
		else
			g_last_exit_code = u_cd_default(data);
	}
	else
		g_last_exit_code = u_cd_home(data);
	return (0);
}
