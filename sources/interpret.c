/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:42:48 by habernar          #+#    #+#             */
/*   Updated: 2024/10/13 20:56:56 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_cmd(t_shell *shell, t_astnode *n)
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
		return ((void)perror("fork"), shell->exit_code = 1, (void)0);
	if (n->cmd->pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		handle_fd(n);
		shell->exit_code = execve(n->cmd->path, n->cmd->params, shell->env);
		perror("execve");
		exit_shell(shell);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		wait_command(shell, n->cmd);
	}
}

static bool	child_birth(t_shell *shell, t_astnode *n, int *pipefd, int *pid)
{
	pid[0] = fork();
	if (pid[0] == -1)
		return (perror("fork"), close(pipefd[1]), close(pipefd[0]), false);
	if (pid[0] == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		ast_interpret(shell, n->left);
		exit_shell(shell);
	}
	pid[1] = fork();
	if (pid[1] == -1)
		return (perror("fork"), close(pipefd[0]), close(pipefd[1]),
			waitpid(pid[0], 0, 0), false);
	if (pid[1] == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		ast_interpret(shell, n->right);
		exit_shell(shell);
	}
	return (true);
}

static void	execute_pipe(t_shell *shell, t_astnode *n)
{
	int	pipefd[2];
	int	pid[2];
	int	status;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), shell->exit_code = 1, (void)0);
	if (!child_birth(shell, n, pipefd, pid))
		return (shell->exit_code = 1, (void)0);
	close(pipefd[1]);
	close(pipefd[0]);
	waitpid(pid[0], &status, 0);
	waitpid(pid[1], &status, 0);
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	else
		shell->exit_code = 128 + WTERMSIG(status);
	g_sigint = 0;
}

static void	execute_bracket(t_shell *shell, t_astnode *n)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == -1)
		return ((void)perror("fork"), shell->exit_code = 1, (void)0);
	else if (pid == 0)
	{
		ast_interpret(shell, n->left);
		exit_shell(shell);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		else
			shell->exit_code = 128 + WTERMSIG(status);
		g_sigint = 0;
	}
}

void	ast_interpret(t_shell *shell, t_astnode *n)
{
	if (!n)
		return ;
	if (n->type == LOGICAL_OR)
	{
		ast_interpret(shell, n->left);
		if (shell->exit_code == 0)
			erase_right_leftmost(&n->right, 0);
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
	else if (n->type == BRACKET)
		execute_bracket(shell, n);
	else if (n->type == CMD)
		execute_cmd(shell, n);
}
