/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:42:54 by habernar          #+#    #+#             */
/*   Updated: 2024/09/15 22:42:56 by habernar         ###   ########.fr       */
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
		return (cmdname);
	item = hashtable_search(shell->ht, "PATH");
	if (!item)
		return (0);
	paths = ft_split(item->value, ':');
	idx = 0;
	while (paths && paths[idx])
	{
		path = ft_strjoin_slash(paths[idx], cmdname, '/');
		if (access(path, F_OK) == 0)
			return (free_tab(paths), path);
		free(path);
		idx++;
	}
	return (free_tab(paths), NULL);
}

t_cmd	*init_command(t_shell *shell)
{
	t_cmd	*c;

	c = (t_cmd *)malloc(sizeof(t_cmd));
	if (!c)
	{
		printf("Error: malloc failed on line %d in file %s", __LINE__, __FILE__);
		shell->exit_code = 1;
		c = 0;
		return (0);
	}
	c->path = 0;
	c->params = 0;
	c->delimiter = 0;
	c->infile = 0;
	c->outfile = 0;
	c->fdin = -1;
	c->fdout = -1;
	c->heredoc = false;
	c->redirout = false;
	c->redirin = false;
	c->redirappend = false;
	return (c);
}

t_cmd	*make_command(t_shell *shell, t_astnode *n, char *str)
{
	t_cmd	*cmd;	

    (void)n;
	cmd = init_command(shell);
	if (!cmd)
	{
		shell->exit_code = 1;
		return (0);
	}
	get_redirs(cmd, str);
	remove_redirs(str);
	remove_whitespace(&str);
    /*
	if (!*str)
	{
        n = 0;
		free_cmd(cmd);
		shell->exit_code = 1;
		exit_shell(shell);
	}
    */
	cmd->params = make_params(str);
    /*
	if (!cmd->params || !*cmd->params)
	{ printf("Error: failed to generate command parameters\n");
		free(str);
        free(n);
        n = 0;
		free_cmd(cmd);
		shell->exit_code = 1;
		exit_shell(shell);
	}
	int i = 0;
	while (cmd->params[i])
		printf("%s\n", cmd->params[i++]);
    */
	cmd->path = make_path(shell, cmd->params[0]);
    /*
	if (!cmd->path)
	{
		printf("Error: failed to generate command path\n");
		free(str);
        free(n);
        n = 0;
		free_cmd(cmd);
		shell->exit_code = 1;
		exit_shell(shell);
	}
    */
	expand_quotes(cmd->params);
	expand_env_variables(shell ,cmd->params);
	return (cmd);
}
