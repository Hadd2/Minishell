#include "minishell.h"

static char	*ft_strndup(char *str, int len)
{
	char	*ret;
	char	*head;

	if (len == 0)
		return (0);
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (0);
	head = ret;
	while (*str && len--)
		*ret++ = *str++;
	*ret = 0;
	return (head);
}

static void	init_shell(t_shell *shell, char **env)
{
	int		i;
	char	*k;
	char	*v;

	shell->exit_code = 0;
	shell->cl = 0;
	shell->ast = 0;
	shell->ht = hashtable_init();
	shell->env = env;
	i = 0;
	while (env && env[i])
	{
        k = ft_strndup(env[i], ft_strchr(env[i], '=') - env[i]);
        v = ft_strdup(ft_strchr(env[i], '=') + 1);
        hashtable_insert(shell->ht, k, v);
        i++;
    }
}

static int	parsable(char *str)
{
	int		ascii[127];
	int		i;
	char	c;

	if (!str || (*str && *str == '\n' && !*(str + 1)))
		return (0);
	i = 0;
	while (i < 127)
		ascii[i++] = 0;
	while (*str)
	{
		ascii[(int)*str]++;
		if (*str == '\'' || *str == '\"')
		{
			c = *str++;
			while (*str && *str != c)
				str++;
			if (*str == c)
				ascii[(int)*str++]++;
		}
		else 
			str++;
	}
	//printf("%d \"\n%d (\n%d )\n%d \'\n", ascii['\"'], ascii['('], ascii[')'], ascii['\'']);
	return (!ascii['\\'] && !ascii[';'] && ascii['\''] % 2 == 0 && ascii['\"'] % 2 == 0 && ascii['('] == ascii[')']);
}

int main(int argc, char **argv, char **env)
{
	t_shell shell;

	(void)argc, (void)argv;
	init_shell(&shell, env);
	//shell.cl = readline("$");
	shell.cl = get_next_line(STDIN_FILENO);
	while (shell.cl)
	{
		shell.headcl = shell.cl;
		if (!parsable(shell.cl))
		{
			free(shell.headcl);
			shell.headcl = 0;
			shell.cl = get_next_line(STDIN_FILENO);
			//shell.cl = readline("$");
			continue;
		}
		shell.ast = parse_logical(&shell, &shell.cl);
		print_ast(shell.ast);
		if (shell.exit_code == 0)
			ast_interpret(&shell, shell.ast);
		free(shell.headcl);
		free_ast(shell.ast);
		shell.ast = 0;
		shell.headcl = 0;
		shell.cl = get_next_line(STDIN_FILENO);
	}
	exit_shell(&shell);
	return (0);
}

/* TODO SHELL STRUCTURE
 * SIGNAL
 * ENV + ENVVARIABLE
 * HANDLE "" AND '' AND $
 * FINISH EXEC
 * BUILTIN
 * HISTORY
 */
