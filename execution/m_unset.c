/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 19:38:32 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 23:01:25 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	u_unset(t_data *data, char *unset)
{
	char	**split;

	split = split_equal(unset);
	if (!u_invalid("minishell: unset: `", unset))
	{
		if (!ft_strcmp(unset, "OLDPWD"))
		{
			free(data->old_working_dir);
			data->old_working_dir = NULL;
		}
		delete_var_env(data, split[0], del_env_content);
	}
	free_arr(split);
}

int	m_unset(t_data *data)
{
	int	i;

	i = 0;
	if ((*data->head_of_commands)->arguments[1])
	{
		while ((*data->head_of_commands)->arguments[++i])
			u_unset(data, (*data->head_of_commands)->arguments[i]);
	}
	return (0);
}
