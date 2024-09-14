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

void	get_here_doc(t_shell *shell, t_cmd *cmd)
{
	char	*buffer;
	int		fd_infile;

	fd_infile = open(TMP_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd_infile == -1)
	{
		printf("Error: failed to open heredoc file on line %d in file %s\n", __LINE__, __FILE__);
		return (shell->exit_code = 1, (void)0);
	}
	while (1)
	{
		printf("> ");
		buffer = get_next_line(STDIN_FILENO);
		if (buffer)
		{
			if (!ft_strncmp(cmd->delimiter, buffer, ft_strlen(cmd->delimiter))
				&& buffer[ft_strlen(cmd->delimiter)] == '\n')
			{
				free(buffer);
				break ;
			}
			write(fd_infile, buffer, ft_strlen(buffer));
			free(buffer);
		}
	}
	close(fd_infile);
}

void	open_file(t_shell *shell, t_cmd *cmd)
{
	/*
	 * infile
	 * MUST BE ANOTHER FOR OUTFILE OTHERWISE RETURN ONLY OUTFILE WHEN BOTH IN AND OUTFILE */
	if (cmd->heredoc || cmd->redirin)
	{
		if (cmd->heredoc)
			cmd->fdin = open(TMP_FILENAME, O_RDONLY);
		else if (cmd->redirin)
			cmd->fdin = open(cmd->infile, O_RDONLY);
		if (cmd->fdin == -1)
		{
			printf("Error: failed to open file descriptor.\n");
			return (shell->exit_code = 1, (void)0);
		}
	}
	if (cmd->redirout || cmd->redirappend)
	{
		if (cmd->redirappend)
			cmd->fdout = open(cmd->outfile, O_CREAT | O_APPEND | O_WRONLY, 0666);
		else if (cmd->redirout)
			cmd->fdout = open(cmd->outfile, O_CREAT | O_TRUNC | O_WRONLY, 0666);
		if (cmd->fdout == -1)
		{
			printf("Error: failed to open file descriptor.\n");
			return (shell->exit_code = 1, (void)0);
		}
	}
}

void handle_fd(t_shell *shell, t_astnode *n)
{
	open_file(shell, n->cmd);
	if (n->pipein != -1)
	{
		dup2(n->pipein, STDIN_FILENO);
		close(n->pipein);
		n->pipein = -1;
	}
	if (n->pipeout != -1)
	{
		dup2(n->pipeout, STDOUT_FILENO);
		close(n->pipeout);
		n->pipeout = -1;
	}
	if (n->cmd->redirin || n->cmd->heredoc)
	{
		if (n->cmd->heredoc)
			get_here_doc(shell, n->cmd);
		if (n->cmd->fdin != -1)
		{
			dup2(n->cmd->fdin, STDIN_FILENO);
			close(n->cmd->fdin);
		}
	}
	if (n->cmd->redirout || n->cmd->redirappend)
	{
		if (n->cmd->fdout != -1)
		{
			dup2(n->cmd->fdout, STDOUT_FILENO);
			close(n->cmd->fdout);
		}
	}
}

/* debug */
void 	print_cmd(t_cmd *cmd)
{
	if (cmd->params)
	{
		int i = 0;
		while (cmd->params[i])
			printf("%s\n", cmd->params[i++]);
	}
	if (cmd->heredoc && cmd->delimiter)
		printf("%s\n", cmd->delimiter);
	if (cmd->infile)
		printf("%s\n", cmd->infile);
	if (cmd->outfile)
		printf("%s\n", cmd->outfile);
	if (cmd->path)
		printf("%s\n", cmd->path);
}

void	parent_close_pipe(t_astnode *n)
{
	if (n->pipein != -1)
	{
		close(n->pipein);
		n->pipein = -1;
	}
	if (n->pipeout != -1)
	{
		close(n->pipeout);
		n->pipeout = -1;
	}
}

void	execute_cmd(t_shell *shell, t_astnode *n)
{
	n->cmd = make_command(shell, n, n->ps);
	if (!n->cmd)
		return ;
	//debug
	print_cmd(n->cmd);
	/*
	if (BUILTIN)
	 */
	n->cmd->pid = fork();
	if (n->cmd->pid == -1)
	{
		printf("Error: failed to create a pipe on line %d in file %s\n", __LINE__, __FILE__);
		return (shell->exit_code = 1, (void)0);
	}
	if (n->cmd->pid == 0)
	{
		handle_fd(shell, n);
		shell->exit_code = execve(n->cmd->path, n->cmd->params, shell->env);
		printf("Error: failed to execve on line %d in file %s\n", __LINE__, __FILE__);
		exit_shell(shell);
	}
	else
	{
		parent_close_pipe(n);
		wait_command(shell, n->cmd);
	}
}

/* POSSIBLE ERROR left node PIPE (cmd1 && cmd2) | cmd3 */
void	execute_pipe(t_shell *shell, t_astnode *n)
{
	int			pipefd[2];

	if (pipe(pipefd) == -1)
	{
		printf("Failed to create pipe on line %d in file %s", __LINE__, __FILE__);
		return (shell->exit_code = 1, (void)0);
	}
	find_all_leaf_left(n, pipefd[1]);
	find_right_leftmost(n->right, pipefd[0]);
	ast_interpret(shell, n->left);
	ast_interpret(shell, n->right);
}

void ast_interpret(t_shell *shell, t_astnode *n)
{
	if (n->type == LOGICAL_OR)
	{
		/* EXEC ONLY IF NODE LEFT SUCCEED */
		ast_interpret(shell, n->left);
		if (shell->exit_code != 0)
			ast_interpret(shell, n->right);
	}
	else if (n->type == LOGICAL_AND)
	{
		/* if left OK and right OK -> exitcode == 0 
		 * if left KO  DONT EXECUTE right -> exitcode != 0
		 * if left OK and right KO -> exitcode != 0 */
		ast_interpret(shell, n->left);
		if (shell->exit_code == 0)
			ast_interpret(shell, n->right);
	}
	else if (n->type == PIPE)
	{
		execute_pipe(shell, n);
	}
	else if (n->type == CMD)
	{
		execute_cmd(shell, n);
	}
}

