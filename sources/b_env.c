/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 11:48:25 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/15 20:18:55 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// print env
static void	b_env_print(t_hashtable *ht, t_shell *shell)
{
	int		i;
	t_item	*item;

	i = 0;
	while (i < ht->size)
	{
		item = ht->items[i];
		if (item && item->key && item->value && !item->tombstone)
		{
			if ((ft_strcmp(item->key, "PATH") == 0) && shell->no_env)
			{
				i++;
				continue ;
			}
			ft_putstr_fd(item->key, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(item->value, 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
}

// option '-' -> print error
// if shell->ht is empty -> print error
void	b_env(char **av, t_shell *shell)
{
	if (av[1] && av[1][0] == '-')
	{
		ft_putstr_fd("env: no options supported\n", 2);
		shell->exit_code = 125;
		return ;
	}
	b_env_print(shell->ht, shell);
	shell->exit_code = 0;
}
