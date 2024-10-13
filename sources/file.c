/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:42:29 by habernar          #+#    #+#             */
/*   Updated: 2024/10/13 18:33:48 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_here_doc(t_shell *shell, t_cmd *cmd, char *delimiter)
{
	char	*buffer;
	int		fd;

	fd = open(TMP_FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
		return (cmd->error = 1, 0);
	while (1)
	{
		buffer = get_next_line(STDIN_FILENO);
		if (g_sigint)
			return (sigint_heredoc(shell, cmd, buffer, fd));
		if (!buffer)
			return ((void)close(fd), 1);
		if (!ft_strncmp(delimiter, buffer, ft_strlen(delimiter)))
		{
			free(buffer);
			break ;
		}
		write(fd, buffer, ft_strlen(buffer));
		write(fd, "\n", 1);
		free(buffer);
	}
	return ((void)close(fd), 0);
}

static void	open_infile(t_cmd *cmd, t_file *filenode)
{
	int	fd;

	if (filenode->type == HEREDOC)
		fd = open(TMP_FILENAME, O_RDONLY);
	else
		fd = open(filenode->name, O_RDONLY);
	if (fd == -1)
	{
		printf("minishell: %s: No such file or directory\n", filenode->name);
		return (cmd->error = 1, (void)0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void	open_outfile(t_cmd *cmd, t_file *filenode)
{
	int	fd;

	if (filenode->type == REDIRAPPEND)
		fd = open(filenode->name, O_CREAT | O_APPEND | O_WRONLY, 0666);
	else
		fd = open(filenode->name, O_CREAT | O_TRUNC | O_WRONLY, 0666);
	if (fd == -1)
	{
		printf("minishell: %s: No such file or directory\n", filenode->name);
		return (cmd->error = 1, (void)0);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	open_file(t_cmd *cmd, t_list *lst)
{
	t_file	*filenode;

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
	t_list	*tmp;

	tmp = n->cmd->lstfiles;
	while (tmp)
	{
		open_file(n->cmd, tmp);
		tmp = tmp->next;
	}
}
