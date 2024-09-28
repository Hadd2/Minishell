/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:40:13 by habernar          #+#    #+#             */
/*   Updated: 2024/09/28 16:27:32 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_cmd(t_cmd *cmd)
{
	t_list	*tmp;
	t_file	*fnode;
	int		i;

	if (cmd->params)
	{
		i = 0;
		while (cmd->params[i])
		{
			printf("param: %d, %s\n", i, cmd->params[i]);
			i++;
		}
	}
	if (cmd->path)
		printf("path:%s\n", cmd->path);
	tmp = cmd->lstfiles;
	while (tmp)
	{
		if (!tmp->content)
			return ;
		fnode = (t_file *)tmp->content;
		printf("type:%d\tname:%s\n", fnode->type, fnode->name);
		tmp = tmp->next;
	}
}

void	print_ast(t_astnode *n)
{
	if (!n)
		return ;
	if (n->type == LOGICAL_AND)
		printf("ast_node logical AND\n");
	if (n->type == LOGICAL_OR)
		printf("ast_node logical OR\n");
	if (n->type == PIPE)
		printf("ast_node PIPE\n");
	if (n->type == CMD)
		printf("ast_node CMD: %s\n", n->ps);
	if (n->left)
	{
		printf("left node");
		print_ast(n->left);
	}
	if (n->right)
	{
		printf("right node");
		print_ast(n->right);
	}
}
