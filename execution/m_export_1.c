/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_export_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 19:38:24 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 22:58:54 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

char	**copy_env(t_data *data)
{
	char	**envp;
	t_env	*node;
	int		i;

	node = *(data->head_env);
	envp = (char **)malloc(sizeof(char *) * (env_len(data->head_env) + 1));
	if (!envp)
		return (NULL);
	i = 0;
	while (node)
	{
		envp[i] = ft_strjoin(node->var, "=");
		envp[i] = ft_strjoin_free(envp[i], node->value);
		i++;
		node = node->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	**split_equal(char *new)
{
	char		**split;
	t_garbage	garb;

	split = (char **)malloc(sizeof(char *) * 3);
	if (!split)
		return (NULL);
	garb.i = -1;
	garb.j = 0;
	while (new[++garb.i] && new[garb.i] != '=')
		;
	split[0] = (char *)malloc(sizeof(char) * (garb.i + 1));
	split[1] = (char *)malloc(sizeof(char) * (ft_strlen(new) - garb.i + 1));
	if (!split[0] || !split[1])
		return (NULL);
	garb.i = -1;
	while (new[++garb.i] && new[garb.i] != '=')
		split[0][garb.i] = new[garb.i];
	split[0][garb.i] = '\0';
	if (new[garb.i] == '=')
		garb.i++;
	while (new[garb.i])
		split[1][garb.j++] = new[garb.i++];
	split[1][garb.j] = '\0';
	split[2] = NULL;
	return (split);
}

int	m_export(t_data *data)
{
	int	i;

	i = 0;
	if (!(*data->head_of_commands)->arguments[1])
		u_declare_print(data);
	if ((*data->head_of_commands)->arguments[1])
	{
		while ((*data->head_of_commands)->arguments[++i])
			add_env(data, (*data->head_of_commands)->arguments[i]);
	}
	g_last_exit_code = 0;
	return (g_last_exit_code);
}
