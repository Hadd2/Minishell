/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 19:27:43 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/02 17:28:20 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// unset alone -> return
// unset key -> delete the key in hastable
void	b_unset(char **av, t_shell *shell)
{
	int		i;
	char	*key;

	if (!av[1])
	{
		shell->exit_code = 0;
		return ;
	}
	i = 1;
	shell->exit_code = 0;
	while (av[i])
	{
		key = av[i];
		if (shell->ht)
			hashtable_delete(shell->ht, key);
		i++;
	}
	shell->exit_code = 0;
}
