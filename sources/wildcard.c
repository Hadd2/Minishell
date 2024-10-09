/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:05:31 by habernar          #+#    #+#             */
/*   Updated: 2024/10/09 19:35:06 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	contains_wildcard(char *str)
{
	char	c;

	while (*str)
	{
		if (*str == '\'' || *str == '\"')
		{
			c = *str++;
			while (*str && *str != c)
				str++;
		}
		else if (*str == '*')
			return (true);
		str++;
	}
	return (false);
}

void	find_match(t_trie *node, char *pattern, t_buffer *b, char ***res)
{
	uint8_t	x;

	if (!*pattern && node->isword && !tab_contains(*res, b->s))
		return (tab_append(res, b));
	if (*pattern == '*')
	{
		x = 0;
		while (x < ALPHSIZE)
		{
			if (node->children[x])
			{
				b->s[b->i++] = x;
				find_match(node->children[x], pattern, b, res);
				b->i--;
			}
			x++;
		}
		find_match(node, pattern + 1, b, res);
	}
	else if (node->children[(uint8_t) * pattern])
	{
		b->s[b->i++] = (uint8_t) * pattern;
		find_match(node->children[(uint8_t) * pattern], pattern + 1, b, res);
	}
}

char	**match_wildcard(char *str, t_trie *root)
{
	char		**res;
	t_buffer	buffer;
	int			i;

	res = 0;
	buffer.i = 0;
	i = 0;
	while (i < 256)
		buffer.s[i++] = 0;
	find_match(root, str, &buffer, &res);
	return (res);
}

char	**expand_parameters(char **params, char **wc, int idx)
{
	char	**res;
	int		i;
	int		j;

	res = (char **)malloc(sizeof(char *) * (tab_size(params) + tab_size(wc)));
	if (!res)
		return (perror("malloc"), free_tab(wc), (char **)0);
	i = 0;
	j = 0;
	while (params && params[i] && i < idx)
		res[j++] = params[i++];
	i = 0;
	while (wc && wc[i])
		res[j++] = wc[i++];
	i = idx + 1;
	while (params && params[i])
		res[j++] = params[i++];
	res[j] = 0;
	free(wc);
	free(params[idx]);
	free(params);
	return (res);
}

void	expand_wildcard(t_cmd *cmd)
{
	int		i;
	char	**res;
	t_trie	*root;

	root = 0;
	search_files(&root);
	i = 1;
	while (cmd->params && cmd->params[i])
	{
		if (contains_wildcard(cmd->params[i]))
		{
			res = match_wildcard(cmd->params[i], root);
			if (!res)
				return (cmd->error = 1, trie_free(root), (void)0);
			cmd->params = expand_parameters(cmd->params, res, i);
		}
		i++;
	}
	trie_free(root);
}
