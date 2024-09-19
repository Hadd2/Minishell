/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:42:29 by habernar          #+#    #+#             */
/*   Updated: 2024/09/19 21:08:17 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_here_doc(t_cmd *cmd, char *delimiter)
{
	char	*buffer;
	int		fd;

    unlink(TMP_FILENAME);
	fd = open(TMP_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
		return (cmd->error = 1, (void)0);
	while (1)
	{
		write(STDIN_FILENO,"> ", 2);
		buffer = get_next_line(STDIN_FILENO);
		if (buffer)
		{
			if (!ft_strncmp(delimiter, buffer, ft_strlen(delimiter))
				&& buffer[ft_strlen(delimiter)] == '\n')
			{
				free(buffer);
				break ;
			}
			write(fd, buffer, ft_strlen(buffer));
			free(buffer);
		}
	}
	close(fd);
}

void	open_infile(t_cmd *cmd, t_file *filenode)
{
	int	fd;

	if (filenode->type == HEREDOC)
		fd = open(TMP_FILENAME, O_RDONLY);
	else
		fd = open(filenode->name, O_RDONLY);
	if (fd == -1)
	{
		printf("bash: %s: No such file or directory\n", filenode->name);
		return (cmd->error = 1, (void)0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	open_outfile(t_cmd *cmd, t_file *filenode)
{
	int	fd;

	if (filenode->type == REDIRAPPEND)
		fd = open(filenode->name, O_CREAT | O_APPEND | O_WRONLY, 0666);
	else
		fd = open(filenode->name, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
	{
		printf("bash: %s: No such file or directory\n", filenode->name);
		return (cmd->error = 1, (void)0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	open_file(t_cmd *cmd, t_list *lst)
{
    t_file  *filenode;

    if (!lst->content)
        return (cmd->error = 1, (void)0);
    filenode = (t_file *)lst->content;
    if (filenode->type == HEREDOC || filenode->type == REDIRIN)
		open_infile(cmd, filenode);
    else if (filenode->type == REDIROUT || filenode->type == REDIRAPPEND)
		open_outfile(cmd, filenode);
}

void	handle_fd(t_astnode *n)
{
    t_list  *tmp;

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
    tmp = n->cmd->lstfiles;
    while (tmp)
    {
        open_file(n->cmd, tmp);
        tmp = tmp->next;
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
