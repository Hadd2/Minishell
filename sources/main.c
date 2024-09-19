/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:29 by habernar          #+#    #+#             */
/*   Updated: 2024/09/19 19:32:04 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_shell(t_shell *shell, char **env)
{
	int		i;
	char	*k;
	char	*v;

	shell->exit_code = 0;
	shell->parse_error = 0;
	shell->cl = 0;
	shell->headcl = 0;
	shell->ast = 0;
	shell->ht = hashtable_init();
	shell->env = env;
	i = 0;
	while (shell->ht && env && env[i])
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

	if (!str || (str && nothing_to_parse(str, *str)))
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
	return (!ascii['\\'] && !ascii[';'] && ascii['\''] % 2 == 0 && ascii['\"'] % 2 == 0 && ascii['('] == ascii[')']);
}

int main(int argc, char **argv, char **env)
{
	t_shell shell;

	(void)argc, (void)argv;
	init_shell(&shell, env);
	//shell.cl = readline("$");
	//char str[200];
	//size_t size = 200;
	shell.cl = get_next_line(STDIN_FILENO);
	while (shell.cl)
	{
		//printf("%s> ", getcwd(str,	size));
		//shell.cl = get_next_line(STDIN_FILENO);
		shell.parse_error = 0;
		shell.headcl = shell.cl;
		if (!parsable(shell.cl))
		{
			free(shell.headcl);
			shell.headcl = 0;
			shell.cl = get_next_line(STDIN_FILENO);
			continue;
		}
		shell.ast = parse_logical(&shell, &shell.cl);
        //print_ast(shell.ast);
		if (shell.parse_error == 0)
			ast_interpret(&shell, shell.ast);
		free(shell.headcl);
		free_ast(shell.ast);
		shell.ast = 0;
		shell.headcl = 0;
		shell.cl = get_next_line(STDIN_FILENO);
	}
	hashtable_free(shell.ht);
	//return (shell.exit_code);
	return (0);
}
