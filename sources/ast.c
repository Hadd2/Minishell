/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:05 by habernar          #+#    #+#             */
/*   Updated: 2024/09/28 15:40:27 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_astnode	*ast_make_node(t_shell *shell, int type, t_astnode *l, t_astnode *r)
{
	t_astnode	*n;

	n = (t_astnode *)malloc(sizeof(t_astnode));
	if (!n)
	{
		perror("malloc");
		return (shell->parse_error = 1, (t_astnode *)0);
	}
	n->type = type;
	n->left = l;
	n->right = r;
	n->ps = 0;
	n->cmd = 0;
	n->pipein = -1;
	n->pipeout = -1;
	return (n);
}

t_astnode	*ast_make_cmd(t_shell *shell, char *s)
{
	t_astnode	*n;

	n = (t_astnode *)malloc(sizeof(t_astnode));
	if (!n)
	{
		perror("malloc");
		return (shell->parse_error = 1, (t_astnode *)0);
	}
	n->type = CMD;
	n->left = 0;
	n->right = 0;
	n->ps = s;
	n->cmd = 0;
	n->pipein = -1;
	n->pipeout = -1;
	return (n);
}

void	find_right_leftmost(t_astnode *n, int fd)
{
	if (!n)
		return ;
	if (n->left && n->type != CMD)
		find_right_leftmost(n->left, fd);
	if (n->type == CMD)
		n->pipein = fd;
}

void	find_all_leaf_left(t_astnode *n, int fd)
{
	if (!n)
		return ;
	else if (n->type == CMD)
		n->pipeout = fd;
	else if (n->left)
		find_all_leaf_left(n->left, fd);
	else if (n->right)
		find_all_leaf_left(n->right, fd);
}
