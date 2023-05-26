/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_export_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkamal <bkamal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/09 11:31:53 by bkamal            #+#    #+#             */
/*   Updated: 2023/02/20 22:59:36 by bkamal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

static void	sort_arr(char **env_arr)
{
	char	*temp;
	int		size;
	int		i;
	int		j;

	i = -1;
	size = env_variable_count(env_arr);
	while (++i < size)
	{
		j = -1;
		while (++j < size - i - 1)
		{
			if (ft_strcmp(env_arr[j], env_arr[j + 1]) > 0)
			{
				temp = env_arr[j];
				env_arr[j] = env_arr[j + 1];
				env_arr[j + 1] = temp;
			}
		}
	}
}

int	after_equal(char *env)
{
	int	flag;
	int	after;
	int	i;

	i = -1;
	flag = 0;
	while (env[++i])
	{
		if (flag == 1)
			after++;
		if (env[i] == '=')
			flag = 1;
	}
	return (after);
}

static void	print_arr(t_exec *exec, char **env_arr)
{
	int	i;
	int	j;

	i = -1;
	while (env_arr[++i])
	{
		j = -1;
		ft_putstr_fd("declare -x ", exec->fd);
		while (env_arr[i][++j])
		{
			if (after_equal(env_arr[i]) == 1 && env_arr[i][j] == '=')
				break ;
			else
				ft_putchar_fd(env_arr[i][j], exec->fd);
			if (env_arr[i][j] == '=')
				ft_putchar_fd('"', exec->fd);
		}
		if (after_equal(env_arr[i]) != 1)
			ft_putchar_fd('"', exec->fd);
		ft_putchar_fd('\n', exec->fd);
	}
}

void	u_declare_print(t_data *data)
{
	char	**sorted;

	sorted = copy_env(data);
	sort_arr(sorted);
	print_arr(data->exec, sorted);
	free_arr(sorted);
}
