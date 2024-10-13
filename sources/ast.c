/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:05 by habernar          #+#    #+#             */
/*   Updated: 2024/10/13 19:24:51 by habernar         ###   ########.fr       */
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
	return (n);
}

void	erase_right_leftmost(t_astnode **n, int count)
{
	if (!*n)
		return ;
	if ((*n)->left && (*n)->type != CMD)
		erase_right_leftmost(&(*n)->left, count + 1);
	if ((*n)->right && (*n)->type != CMD && count > 0)
		erase_right_leftmost(&(*n)->right, count + 1);
	if ((*n)->type == CMD)
	{
		if ((*n)->cmd)
			free_cmd((*n)->cmd);
		if ((*n)->ps)
			free((*n)->ps);
		free(*n);
		*n = 0;
	}
}
