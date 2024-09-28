/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:29 by habernar          #+#    #+#             */
/*   Updated: 2024/09/28 17:23:19 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint = 0;

static void	init_shell(t_shell *shell, int argc, char **argv, char **env)
{
	int		i;
	char	*k;
	char	*v;

	(void)argc, (void)argv;
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
	setup_signal();
}

static int	parsable(char *str)
{
	int		ascii[127];
	int		i;
	char	c;

	if (!str || (str && nothing_to_parse(str)))
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

static void	do_logic(t_shell *shell)
{
	add_history(shell->cl);
	shell->parse_error = 0;
	shell->headcl = shell->cl;
	shell->ast = parse_logical(shell, &shell->cl);
	if (shell->parse_error == 0)
		ast_interpret(shell, shell->ast);
	free(shell->headcl);
	free_ast(shell->ast);
	shell->ast = 0;
	shell->headcl = 0;
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	init_shell(&shell, argc, argv, env);
	while (1)
	{
		shell.cl = readline("minishell$ ");
		if (g_sigint)
		{
			shell.exit_code = 130;
			g_sigint = 0;
			if (shell.cl)
				free(shell.cl);
			continue ;
		}
		if (!shell.cl)
			break ;
		if (!parsable(shell.cl))
		{
			free(shell.cl);
			continue ;
		}
		do_logic(&shell);
	}
	exit_shell(&shell);
}
