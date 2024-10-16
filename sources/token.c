/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 19:14:18 by habernar          #+#    #+#             */
/*   Updated: 2024/10/16 15:19:13 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_tok(char **str, t_tok *tok)
{
	const char	*tokens[] = {"&&", "||", "|", "<<",
		">>", "<", ">", "(", ")", 0};
	int			i;

	skip_whitespace(str);
	skip_quotes(str);
	if (!str || !*str || !**str)
		return (tok->type = T_EOF, (void)0);
	if (ft_isalnumsup(**str))
	{
		while (ft_isalnumsup(**str))
			(*str)++;
		return (tok->type = T_ALNUM, (void)0);
	}
	i = 0;
	while (i < 9)
	{
		if (!ft_strncmp(*str, tokens[i], ft_strlen(tokens[i])))
			return (tok->type = i, *str += ft_strlen(tokens[i]), (void)0);
		i++;
	}
}

static void	token_error(t_tok prev, t_tok curr)
{
	const char	*tokens[] = {"&&", "||", "|", "<<", ">>",
		"<", ">", "(", ")", 0};

	if ((prev.type >= T_DSUP && prev.type <= T_SINF) && curr.type == T_EOF)
		printf("minishell: syntax error near unexpected token \" newline \"\n");
	else if (curr.type != T_EOF)
		printf("minishell: syntax error near unexpected token \" %s \"\n",
			tokens[curr.type]);
	else
		printf("minishell: syntax error near unexpected token \" %s \"\n",
			tokens[prev.type]);
}

static bool	rules_broken(t_toktype curr, t_toktype prev)
{
	if ((is_operator(curr) && prev == T_UNINIT)
		|| (is_operator(prev) && (curr == T_EOF || curr == T_RIGHTP))
		|| (is_redirection(prev) && curr != T_ALNUM)
		|| (prev == T_LEFTP && is_operator(curr))
		|| (prev == T_RIGHTP && curr == T_LEFTP)
		|| (curr == prev && (prev >= T_AND && prev <= T_SINF)))
		return (true);
	return (false);
}

bool	check_token(char *str)
{
	char	*new;
	char	*head;
	t_tok	curr;
	t_tok	prev;

	new = ft_strdup(str);
	head = new;
	get_tok(&new, &curr);
	prev.type = T_UNINIT;
	while (prev.type != T_EOF)
	{
		if (rules_broken(curr.type, prev.type))
			return (token_error(prev, curr), free(head), false);
		prev = curr;
		get_tok(&new, &curr);
	}
	return (free(head), true);
}
