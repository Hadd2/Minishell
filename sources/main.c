/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:29 by habernar          #+#    #+#             */
/*   Updated: 2024/10/16 15:12:56 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint = 0;

static int	valid_char(char *str)
{
	int		ascii[127];
	char	c;
	int		i;

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
			if (*str && *str == c)
				ascii[(int)*str]++;
		}
		if (*str)
			str++;
	}
	return (!ascii['\\'] && !ascii[';'] && ascii['\''] % 2 == 0
		&& ascii['\"'] % 2 == 0 && ascii['('] == ascii[')']
		&& ascii['&'] != 1);
}

static int	parsable(char *str)
{
	if (!str || nothing_to_parse(str) || !valid_char(str)
		|| !check_token(str))
		return (0);
	return (1);
}

static void	do_logic(t_shell *shell)
{
	shell->parse_error = 0;
	shell->headcl = shell->cl;
	shell->ast = parse_logical(shell, &shell->cl);
	if (shell->parse_error == 0)
		ast_interpret(shell, shell->ast);
	//print_ast(shell->ast);
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
		if (!shell.cl)
			break ;
		if (g_sigint)
		{
			shell.exit_code = 130;
			g_sigint = 0;
			if (shell.cl)
				free(shell.cl);
			continue ;
		}
		add_history(shell.cl);
		if (!parsable(shell.cl))
			free(shell.cl);
		else
			do_logic(&shell);
	}
	exit_shell(&shell);
}
