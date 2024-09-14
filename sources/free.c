#include "minishell.h"

void	free_tab(char **tab)
{
	int	idx;

	idx = 0;
	if (tab)
	{
		while (tab[idx])
		{
			free(tab[idx]);
			idx++;
		}
		free(tab);
	}
}

void 	free_cmd(t_cmd *cmd)
{
	if (cmd->params)
		free_tab(cmd->params);
	if (cmd->heredoc)
	{
		unlink(TMP_FILENAME);
		if (cmd->delimiter)
			free(cmd->delimiter);
	}
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->path)
		free(cmd->path);
	free(cmd);
}

void	free_ast(t_astnode *n)
{
	if (n->left)
		free_ast(n->left);
	if (n->right)
		free_ast(n->right);
	if (n->type == CMD && n->cmd)
		free_cmd(n->cmd);
	if (n->ps)
		free(n->ps);
	if (n->type == PIPE)
	{
		if (n->pipein != -1)
			close(n->pipein);
		if (n->pipeout != -1)
			close(n->pipeout);
	}
	free(n);
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
