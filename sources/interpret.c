/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:42:48 by habernar          #+#    #+#             */
/*   Updated: 2024/09/19 20:33:17 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_command(t_shell *shell, t_cmd *cmd)
{
	int	status;

	if (wait(&status) == cmd->pid)
	{
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		else
			shell->exit_code = 128 + WTERMSIG(status);
	}
}

/* debug */
void 	print_cmd(t_cmd *cmd)
{
    t_list  *tmp;
    t_file  *fnode;

	if (cmd->params)
	{
		int i = 0;
		while (cmd->params[i])
        {
			printf("param: %d, %s\n",i, cmd->params[i]);
            i++;
        }
	}
	if (cmd->path)
		printf("path:%s\n", cmd->path);
    tmp = cmd->lstfiles;
    while (tmp)
    {
        if (!tmp->content)
            return ;
        fnode = (t_file *)tmp->content;
		printf("type:%d\tname:%s\n", fnode->type, fnode->name);
        tmp = tmp->next;
    }
}

void	execute_cmd(t_shell *shell, t_astnode *n)
{
	make_command(shell, n);
	//print_cmd(n->cmd);
	if (n->cmd->error)
		return ;
    /*
    if (is_builtin(...))
        return (exec_builtin(...))
    */
	n->cmd->pid = fork();
	if (n->cmd->pid == -1)
	{
        perror("fork");
		return (shell->exit_code = 1, (void)0);
	}
	if (n->cmd->pid == 0)
	{
		handle_fd(n);
		shell->exit_code = execve(n->cmd->path, n->cmd->params, shell->env);
        perror("execve");
		exit_shell(shell);
	}
	else
	{
		parent_close_pipe(n);
		wait_command(shell, n->cmd);
	}
}

void	execute_pipe(t_shell *shell, t_astnode *n)
{
	int			pipefd[2];

	if (pipe(pipefd) == -1)
	{
        perror("pipe");
		return (shell->exit_code = 1, (void)0);
	}
	find_all_leaf_left(n, pipefd[1]);
	find_right_leftmost(n->right, pipefd[0]);
	ast_interpret(shell, n->left);
	ast_interpret(shell, n->right);
}

void ast_interpret(t_shell *shell, t_astnode *n)
{
	if (!n)
		return ;
	if (n->type == LOGICAL_OR)
	{
		ast_interpret(shell, n->left);
		if (shell->exit_code != 0)
			ast_interpret(shell, n->right);
	}
	else if (n->type == LOGICAL_AND)
	{
		ast_interpret(shell, n->left);
		if (shell->exit_code == 0)
			ast_interpret(shell, n->right);
	}
	else if (n->type == PIPE)
		execute_pipe(shell, n);
	else if (n->type == CMD)
		execute_cmd(shell, n);
}
