/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:42:29 by habernar          #+#    #+#             */
/*   Updated: 2024/09/15 22:42:31 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_fd(t_shell *shell, t_astnode *n)
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
			n->cmd->fdin = -1;
		}
	}
	if (n->cmd->redirout || n->cmd->redirappend)
	{
		if (n->cmd->fdout != -1)
		{
			dup2(n->cmd->fdout, STDOUT_FILENO);
			close(n->cmd->fdout);
			n->cmd->fdout = -1;
		}
	}
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
