/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:39 by habernar          #+#    #+#             */
/*   Updated: 2024/09/28 16:36:50 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_redir_node(t_shell *shell, t_cmd *cmd, int type, char *str)
{
	t_list	*lnode;
	t_file	*fnode;

	fnode = (t_file *)malloc(sizeof(t_file));
	if (!fnode)
	{
		perror("malloc");
		return (cmd->error = 1, (void)0);
	}
	if (type == HEREDOC)
	{
		unlink(TMP_FILENAME);
		if (get_here_doc(shell, cmd, str))
			printf(HEREDOC_EOF, str);
	}
	fnode->name = str;
	fnode->type = type;
	lnode = ft_lstnew(fnode);
	if (!lnode)
		return (cmd->error = 1, (void)0);
	ft_lstadd_back(&cmd->lstfiles, lnode);
}

static int	get_redir_name(t_shell *shell, t_cmd *cmd, char *str, int type)
{
	int		size;
	int		whitespace;
	char	*name;

	size = 0;
	whitespace = 0;
	while (*str && (*str == ' ' || *str == '<' || *str == '>'))
	{
		whitespace++;
		str++;
	}
	while (*(str + size) && *(str + size) != '<' && *(str + size) != '>'
		&& !is_delimiter(*(str + size)) && !is_whitespace(*(str + size)))
		size++;
	name = (char *)malloc(sizeof(char) * (size + 1));
	if (!name)
	{
		perror("malloc");
		return (cmd->error = 1, size + whitespace);
	}
	ft_memcpy(name, str, size);
	*(name + size) = 0;
	get_redir_node(shell, cmd, type, name);
	return (size + whitespace);
}

void	get_redirs(t_shell *shell, t_cmd *cmd, char *str)
{
	while (*str)
	{
		str += ignore_quotes(str);
		if (*str && *str == '<')
		{
			if (*(str + 1) && *(str + 1) == '<')
				str += get_redir_name(shell, cmd, str, HEREDOC);
			else
				str += get_redir_name(shell, cmd, str, REDIRIN);
		}
		else if (*str && *str == '>')
		{
			if (*(str + 1) && *(str + 1) == '>')
				str += get_redir_name(shell, cmd, str, REDIRAPPEND);
			else
				str += get_redir_name(shell, cmd, str, REDIROUT);
		}
		else
			str++;
	}
}

static bool	double_redir_sym(char *str)
{
	if (!ft_strncmp(str, "<<", 2) || !ft_strncmp(str, ">>", 2))
		return (true);
	return (false);
}

char	*remove_redirs(char *str)
{
	int		i;
	int		head;

	i = 0;
	while (*(str + i))
	{
		i += ignore_quotes(str + i);
		if (*(str + i) && (*(str + i) == '<' || *(str + i) == '>'))
		{
			head = i++;
			if (double_redir_sym(str + i - 1))
				i++;
			while (*(str + i) && *(str + i) == ' ')
				i++;
			while (*(str + i) && (*(str + i) != ' '
					&& *(str + i) != '(' && *(str + i) != ')'))
				i++;
			ft_memmove(str + head, str + i, ft_strlen(str + i) + 1);
			i = head;
		}
		else
			i++;
	}
	return (str);
}
