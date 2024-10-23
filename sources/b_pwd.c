/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 10:10:12 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/03 13:33:48 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// malloc buffer, if fail print error and exit_code 1
// if option is -, print an error, exit_code 1, free buf and return
// call getcwd, if error print it, exit_code 1, free buf, return
// print the pwd
void	b_pwd(char **av, t_shell *shell)
{
	char	buffer[1024];
	char	*path;
	int		j;

	if (av[1] != NULL && av[1][0] == '-')
	{
		j = 1;
		while (av[1][j++])
		{
			ft_putstr_fd("minishell: pwd: ", 2);
			ft_putstr_fd(av[1], 2);
			ft_putstr_fd(": invalid option\n", 2);
			return (shell->exit_code = 2, (void)0);
		}
	}
	path = hashtable_get(shell->ht, "PWD");
	if (!path)
	{
		if (!getcwd(buffer, sizeof(buffer)))
			return (perror("getcwd"), shell->exit_code = 1, (void)0);
		path = buffer;
	}
	ft_putstr_fd(path, 1);
	ft_putstr_fd("\n", 1);
	shell->exit_code = 0;
}
