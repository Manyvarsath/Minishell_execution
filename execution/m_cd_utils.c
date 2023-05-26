/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_cd_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 22:41:33 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 22:42:40 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	u_switch(t_data *data, char *dir, t_garbage *garb, t_env *temp)
{
	if (!ft_strncmp(temp->var, "PWD", 4))
	{
		garb->flag = 1;
		garb->temp_pwd = ft_strdup(temp->value);
		free(temp->value);
		temp->value = ft_strdup(dir);
		free(data->working_dir);
		data->working_dir = ft_strdup(temp->value);
	}
	if (!ft_strncmp(temp->var, "OLDPWD", 7) && garb->flag != 0)
	{
		free(temp->value);
		temp->value = ft_strdup(garb->temp_pwd);
		free(garb->temp_pwd);
		garb->temp_pwd = NULL;
		free(data->old_working_dir);
		data->old_working_dir = ft_strdup(temp->value);
	}
}

int	u_env_switch(t_data *data, char *directory)
{
	t_garbage	garb;
	t_env		*temp;

	garb.temp_pwd = NULL;
	data->exec->i = 0;
	temp = (*data->head_env);
	while (temp)
	{
		u_switch(data, directory, &garb, temp);
		temp = temp->next;
	}
	if (garb.temp_pwd)
		free(garb.temp_pwd);
	return (0);
}

int	u_check_pwd(t_data *data)
{
	t_env	*temp;

	temp = (*data->head_env);
	while (temp)
	{
		if (!ft_strncmp(temp->var, "PWD", 4))
			return (1);
		temp = temp->next;
	}
	return (0);
}

int	u_cd_default(t_data *data)
{
	char	pwd[PATH_MAX];
	char	*test;

	if (!chdir((*data->head_of_commands)->arguments[1]))
	{
		if (!getcwd(pwd, sizeof(pwd)))
			error_msg("cd: error retrieving current directory",
				": getcwd: cannot access parentdirectories");
		(*data->head_of_commands)->arguments[1] = ft_strdup(pwd);
		if (!u_check_pwd(data))
		{
			test = ft_strdup((*data->head_of_commands)->arguments[1]);
			lstadd_back_var(data->head_env, lstnew_var(ft_strdup("PWD"), test));
		}
		u_env_switch(data, (*data->head_of_commands)->arguments[1]);
		free((*data->head_of_commands)->arguments[1]);
		return (0);
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd((*data->head_of_commands)->arguments[1], 2);
		ft_putendl_fd(": No such file or directory", 2);
	}
	return (1);
}
