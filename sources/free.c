/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:14 by habernar          #+#    #+#             */
/*   Updated: 2024/09/19 21:09:14 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tab(char **tab)
{
	int	idx;

	idx = 0;
	while (tab[idx])
	{
		free(tab[idx]);
		idx++;
	}
	free(tab);
}

void del_filenode(void *content)
{
    t_file *fnode;

    fnode = (t_file *)content;
	if (!fnode)
		return;
	if (fnode->type == HEREDOC)
		unlink(TMP_FILENAME);
	if (fnode->name)
        free(fnode->name);
    free(fnode);
}

void 	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->params)
		free_tab(cmd->params);
    if (cmd->lstfiles)
        ft_lstclear(&cmd->lstfiles, del_filenode);
    if (cmd->path)
		free(cmd->path);
    free(cmd);
}

void	free_ast(t_astnode *n)
{
	if (!n)
		return ;
	if (n->left)
		free_ast(n->left);
	if (n->right)
		free_ast(n->right);
	if (n->cmd)
		free_cmd(n->cmd);
	if (n->ps)
		free(n->ps);
	if (n->pipein != -1)
	{
		close(n->pipein);
		n->pipein = -1;
	}
	if (n->pipeout != -1)
	{
		close(n->pipeout);
		n->pipeout = -1;
	}
	free(n);
	n = 0;
}

void	exit_shell(t_shell *shell)
{
	if (shell->headcl)
		free(shell->headcl);
	if (shell->ast)
		free_ast(shell->ast);
	if (shell->ht)
		hashtable_free(shell->ht);
	exit(shell->exit_code);
}
