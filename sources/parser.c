/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:35 by habernar          #+#    #+#             */
/*   Updated: 2024/09/19 18:32:48 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_astnode	*parse_command(t_shell *shell, char **str)
{
	char	*s;
	char	*cmd;

	s = *str;
	skip_whitespace(str);
	skip_quotes(str);
	skip_whitespace(str);
	int len =  *str - s;
	cmd = malloc(sizeof(char) * (len + 1));
	if (!cmd)
	{
		perror("malloc");
		return (shell->parse_error = 1, (t_astnode *)0);
	}
	int i = 0;
	while (i < len)
	{
		*(cmd + i++) = *s++;
	}
	*(cmd + i) = 0;
	return (ast_make_cmd(shell, cmd));
}

static t_astnode	*parse_parenthese(t_shell *shell, char **str)
{
	t_astnode	*head;

	head = 0;
	skip_whitespace(str);
	if (*(*str) == '(')
	{
		(*str)++;
		head = parse_logical(shell, str);
		if (*(*str) != ')')
		{
			printf("Error: parsor detected missing ')'\n");
			return (shell->parse_error = 1, head);
		}
		(*str)++;
	}
	else
		head = parse_command(shell, str);
	return (head);
}

static t_astnode	*parse_pipe(t_shell *shell, char **str)
{

	t_astnode	*left;
	t_astnode	*right;

	left = 0;
	right = 0;
	skip_whitespace(str);
	left = parse_parenthese(shell, str);
	skip_whitespace(str);
	if (left && (*(*str) == '|' && *(*str + 1) != '|'))
	{
		(*str)++;
		if (nothing_to_parse(*str, '|'))
			return (shell->parse_error = 1, left);
		right = parse_pipe(shell, str);
		left = ast_make_node(shell, PIPE, left, right);
	}
	return (left);
}

t_astnode	*parse_logical(t_shell *shell, char **str)
{
	t_astnode	*left;
	t_astnode	*right;
	int			type;

	left = 0;
	right = 0;
	skip_whitespace(str);
	left = parse_pipe(shell, str);
	skip_whitespace(str);
	if (left && ((*(*str) == '&' && *(*str + 1) == '&') || (*(*str) == '|' && *(*str + 1) == '|')))
	{
		if (*(*str) == '&')
			type = LOGICAL_AND;
		else
			type = LOGICAL_OR;
		(*str) += 2;
		if (nothing_to_parse(*str, *(*str - 2)))
			return (shell->parse_error = 1, left);
		right = parse_logical(shell, str);
		left = ast_make_node(shell, type, left, right);
	}
	return (left);
}
