/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:42:54 by habernar          #+#    #+#             */
/*   Updated: 2024/09/19 19:26:17 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_quotes(char **tab)
{
	int	i;
	char *str;
	char quotes;

	i = 0;
	while (tab && tab[i])
	{
		if (ft_strchr(tab[i], '\'') != 0)
		{
			str = tab[i];
			while (*str)
			{
				if (*str == '\'')
				{
					quotes = *str++;
					while (*str && *str != quotes)
					{
						if (*str == 31)
							*str = ASCII_SPACE;
						str++;
					}
					if (*str == quotes)
						quotes = 0;
				}
				str++;
			}
		}
		if (ft_strchr(tab[i], '\"') != 0)
		{
			str = tab[i];
			while (*str)
			{
				if (*str == '\"')
				{
					quotes = *str++;
					while (*str && *str != quotes)
					{
						if (*str == 31)
							*str = ASCII_SPACE;
						str++;
					}
					if (*str == quotes)
						quotes = 0;
				}
				str++;
			}
		}
		i++;
	}
}

char	**make_params(char *str)
{
	char	**ret;
	char	quotes;
	char	*head;

	quotes = 0;
	head = str;
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
		{
			quotes = *str++;
			while (*str && *str != quotes)
			{
				if (*str == ASCII_SPACE)
					*str = 31;
				str++;
			}
			if (*str == quotes)
				quotes = 0;
		}
		str++;
	}
	ret = ft_split(head, ASCII_SPACE);
	return (ret);
}

char	*make_path(t_shell  *shell, char *cmdname)
{
	char	**paths;
	char	*path;
	t_item	*item;
	int		idx;

	if (access(cmdname, F_OK) == 0)
		return (ft_strdup(cmdname));
	item = hashtable_search(shell->ht, "PATH");
	if (!item)
		return (0);
	paths = ft_split(item->value, ':');
	idx = 0;
	while (paths && paths[idx])
	{
		path = ft_strjoin_slash(paths[idx], cmdname, '/');
		if (access(path, F_OK) == 0)
			return (free_tabe(paths), path);
		free(path);
		idx++;
	}
	return (free_tabe(paths), (char *)0);
}

t_cmd	*init_command(void)
{
	t_cmd	*c;

	c = (t_cmd *)malloc(sizeof(t_cmd));
	if (!c)
	{
        perror("malloc");
		return (0);
	}
    c->error = 0;
	c->path = 0;
	c->params = 0;
    c->lstfiles = 0;
	return (c);
}

void	make_command(t_shell *shell, t_astnode *n)
{
	n->cmd = init_command();
	if (!n->cmd)
		return (n->cmd->error = 1, (void)0);
	get_redirs(n->cmd, n->ps);
	remove_redirs(n->ps);
	remove_whitespace(&n->ps);
	if (!*n->ps)
        return (n->cmd->error = 1, (void)0);
	n->cmd->params = make_params(n->ps);
	if (!n->cmd->params || !*n->cmd->params)
	{
        printf("Error: failed to generate command parameters\n");
        return (n->cmd->error = 1, (void)0);
	}
	n->cmd->path = make_path(shell, n->cmd->params[0]);
	if (!n->cmd->path)
	{
        printf("bash: %s: command not found\n", n->cmd->params[0]);
        return (n->cmd->error = 1, (void)0);
	}
	expand_quotes(n->cmd->params);
	expand_env_variables(shell ,n->cmd->params);
}
