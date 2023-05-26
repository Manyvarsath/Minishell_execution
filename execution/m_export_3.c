/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_export_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 22:53:36 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 23:00:10 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	u_replace_env(t_env **env, char *new, int *flag)
{
	char	**split;
	t_env	*temp;

	temp = *env;
	split = split_equal(new);
	while (*env)
	{
		if (!ft_strcmp((*env)->var, split[0]))
		{
			free((*env)->value);
			(*env)->value = ft_strdup(split[1]);
			*flag = 1;
			break ;
		}
		*env = (*env)->next;
	}
	*env = temp;
	free_arr(split);
}

void	u_new_env(t_env **env, char *new)
{
	char	**split;

	split = split_equal(new);
	lstadd_back_var(env, lstnew_var(split[0], split[1]));
	free(split);
}

int	u_invalid(char *type, char *new)
{
	int	i;

	i = 0;
	if (ft_isalpha(new[i]) || new[i] == '_')
	{
		while (new[++i] && new[i] != '=')
		{
			if (!(ft_isalnum(new[i]) || new[i] == '_'))
				return (err_export(type, new));
		}
	}
	else
		return (err_export(type, new));
	return (0);
}

void	add_env(t_data *data, char *new)
{
	int	flag;

	flag = 0;
	if (!u_invalid("minishell: export: `", new))
	{
		u_replace_env(data->head_env, new, &flag);
		if (!flag)
			u_new_env(data->head_env, new);
	}
}
