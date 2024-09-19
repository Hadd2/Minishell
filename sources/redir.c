/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:39 by habernar          #+#    #+#             */
/*   Updated: 2024/09/19 19:32:09 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_redir_node(t_cmd *cmd, int type, char *str)
{
    t_list  *lnode;
    t_file  *fnode;

    fnode = (t_file *)malloc(sizeof(t_file));
    if (!fnode)
	{
        perror("malloc");
        return (cmd->error = 1, (void)0);
	}
    fnode->type = type;
    fnode->name = str;
    lnode = ft_lstnew(fnode);
    if (!lnode)
        return (cmd->error = 1, (void)0);
    ft_lstadd_back(&cmd->lstfiles, lnode);
}

static int	get_redir_name(t_cmd *cmd, char *str, int type)
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
	while (*(str + size) && !is_delimiter(*(str + size)) && !is_whitespace(*(str + size)) && *(str + size) != '<' && *(str + size) != '>')
		size++;
	name = (char *)malloc(sizeof(char) * (size + 1));
	if (!name)
	{
        perror("malloc");
        return (cmd->error = 1, size + whitespace);
	}
	ft_memcpy(name, str, size);
	*(name + size) = 0;
	get_redir_node(cmd, type, name);
	return (size + whitespace);
}

void	get_redirs(t_cmd *cmd, char *str)
{
	while (*str)
	{
		if (*str == '<')
		{
			if (*(str + 1) && *(str + 1) == '<')
				str += get_redir_name(cmd, str, HEREDOC);
			else
				str += get_redir_name(cmd, str, REDIRIN);
		}
		else if (*str == '>')
		{
			if (*(str + 1) && *(str + 1) == '>')
				str += get_redir_name(cmd, str, REDIRAPPEND);
			else
				str += get_redir_name(cmd, str, REDIROUT);
		}
		else
			str++;
	}
}

char	*remove_redirs(char *str)
{
	int		i;
	int		head;

	i = 0;
	while (*(str + i))
	{
		if (*(str + i) == '<' || *(str + i) == '>')
		{
			head = i++;
			if (!ft_strncmp(str + i - 1, "<<", 2) || !ft_strncmp(str + i - 1, ">>", 2))
				i++;
            while (*(str + i) && *(str + i) == ' ')
                i++;
            while (*(str + i) && (*(str + i) != ' ' && *(str + i) != '(' && *(str + i) != ')'))
                i++;
			ft_memmove(str + head, str + i, ft_strlen(str + i) + 1);
			i = head;
		}
		else
			i++;
	}
#ifdef DEBUG
	printf("rmredir: %s\n", str);
#endif
	return (str);
}
