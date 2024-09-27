/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trie.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:08:25 by habernar          #+#    #+#             */
/*   Updated: 2024/09/27 22:10:03 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_trie	*trie_create(void)
{
	t_trie	*node;
	uint8_t	i;

	node = (t_trie *)malloc(sizeof(t_trie));
	if (!node)
		return (0);
	i = 0;
	while (i < ALPHSIZE)
		node->children[i++] = 0;
	node->isword = false;
	return (node);
}

void	trie_insert(t_trie **root, char *str)
{
	t_trie	*tmp;
	uint8_t	c;

	if (!*root)
		*root = trie_create();
	tmp = *root;
	while (*str)
	{
		c = (uint8_t) * str;
		if (!tmp->children[c])
			tmp->children[c] = trie_create();
		tmp = tmp->children[c];
		str++;
	}
	tmp->isword = true;
}

bool	trie_search(t_trie *node, char *target)
{
	uint8_t	c;

	if (!node)
		return (false);
	while (*target)
	{
		c = (uint8_t) * target;
		if (!node->children[c])
			return (false);
		node = node->children[c];
		target++;
	}
	return (node && node->isword);
}

void	trie_free(t_trie *node)
{
	uint8_t	i;

	if (!node)
		return ;
	i = 0;
	while (i < ALPHSIZE)
	{
		if (node->children[i])
			trie_free(node->children[i]);
		i++;
	}
	free(node);
}

void	search_files(t_trie **root)
{
	DIR				*dir;	
	struct dirent	*file;

	dir = opendir(".");
	if (!dir)
		return (perror("dir"));
	file = readdir(dir);
	while (file)
	{
		if (ft_strncmp(file->d_name, ".", ft_strlen(file->d_name))
			&& ft_strncmp(file->d_name, "..", ft_strlen(file->d_name)))
			trie_insert(root, file->d_name);
		file = readdir(dir);
	}
	closedir(dir);
}
