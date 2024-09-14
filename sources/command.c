#include "minishell.h"

char	**make_params(char *str)
{
	char	**ret;

	ret = ft_split(str, ASCII_SPACE);
	return (ret);
}

char	*strjoin(char *s1, char *s2, char sep)
{
	char	*ret;
	char	*head;

	ret = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2));
	if (!ret)
		return (0);
	head = ret;
	while (*s1)
		*ret++ = *s1++;
	*ret++ = sep;
	while (*s2)
		*ret++ = *s2++;
	*ret = 0;
	return (head);
}
/*
char	*make_path(char **env, char *cmdname)
{
	char	**paths;
	char	*path;
	int		idx;

	if (access(cmdname, F_OK) == 0)
		return (cmdname);
	while (env && *env)
	{
		if (ft_strncmp(*env, "PATH=", 5) == 0)
			break ;
		env++;
	}
	if (!*env)
		return (0);
	paths = ft_split(*env + 5, ':');
	idx = 0;
	while (paths && paths[idx])
	{
		path = strjoin(paths[idx], cmdname, '/');
		if (access(path, F_OK) == 0)
			return (free_tab(paths), path);
		free(path);
		idx++;
	}
	return (free_tab(paths), NULL);
}
*/

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
		path = strjoin(paths[idx], cmdname, '/');
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
	c->heredoc = FALSE;
	c->redirout = FALSE;
	c->redirin = FALSE;
	c->redirappend = FALSE;
	return (c);
}

void	remove_whitespace(char **str)
{
	char c;
	char *h;

	h = *str;
	while (*(*str))
	{
		c = (*(*str));
		if (c == '\t' || c == '\n' || c == '\v' || c == '\r')
			*(*str) = ' ';
		(*str)++;
	}
	*str = h;
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
	return (cmd);
}
