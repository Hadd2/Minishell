/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_call.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 10:41:01 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/09 14:59:29 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// check if it's a built-in command
int	is_builtin(const char *cmd)
{
	int			i;
	const char	**is_b;

	i = 0;
	is_b = (const char *[])
	{
		"cd", "echo", "env", "exit", "export", "unset", "pwd", NULL
	};
	while (is_b[i])
	{
		if (ft_strcmp(cmd, is_b[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

// call the appropriate function when you type it
	// return 1 if it exists
	// return 0 otherwise
static int	builtins(t_shell *shell, t_astnode *n)
{
	char	*cmd;

	cmd = n->cmd->params[0];
	if (ft_strcmp(cmd, "cd") == 0)
		return (b_cd(n->cmd->params, shell->ht, shell), 1);
	if (ft_strcmp(cmd, "echo") == 0)
		return (b_echo(n->cmd->params, shell), 1);
	if (ft_strcmp(cmd, "env") == 0)
		return (b_env(n->cmd->params, shell), 1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (b_exit(n->cmd->params, shell), 1);
	if (ft_strcmp(cmd, "export") == 0)
		return (b_export(n->cmd->params, shell), 1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (b_pwd(n->cmd->params, shell), 1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (b_unset(n->cmd->params, shell), 1);
	return (0);
}

// execute the built-in command
// save the standard input and output file descriptors
// handle the file descriptors
// call the built-in function
// restore the standard input and output file descriptors
// close the standard input and output file descriptors
void	execute_builtin(t_shell *shell, t_astnode *n)
{
	int	fdstdin;
	int	fdstdout;

	fdstdin = dup(STDIN_FILENO);
	fdstdout = dup(STDOUT_FILENO);
	handle_fd(n);
	builtins(shell, n);
	dup2(fdstdin, STDIN_FILENO);
	dup2(fdstdout, STDOUT_FILENO);
	close(fdstdin);
	close(fdstdout);
}
