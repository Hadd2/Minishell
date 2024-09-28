/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:22:24 by habernar          #+#    #+#             */
/*   Updated: 2024/09/28 16:37:12 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	del_quotes(char *str)
{
	char	quotes;
	char	*head;
	char	*tmp;

	quotes = 0;
	head = str;
	tmp = head;
	while (*str)
	{
		if ((*str == '\'' || *str == '\"') && quotes == 0)
			quotes = *str++;
		else if (*str == quotes)
		{
			quotes = 0;
			str++;
		}
		else
			*tmp++ = *str++;
	}
	*tmp = 0;
	str = head;
}

void	remove_quotes(char **tab)
{
	int		i;

	i = 0;
	while (tab && tab[i])
		del_quotes(tab[i++]);
}

static void	replace_space(char *tab)
{
	char	*str;
	char	quotes;

	str = tab;
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
		{
			quotes = *str++;
			while (*str && *str != quotes)
			{
				if (*str == 31)
					*str = ASCII_SPACE;
				str++;
			}
			if (*str == quotes)
				quotes = 0;
		}
		str++;
	}
}

void	expand_quotes(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		replace_space(tab[i++]);
}

int	ignore_quotes(char *str)
{
	char	quotes;
	int		i;

	i = 0;
	if (*(str + i) == '\'' || *(str + i) == '\"')
	{
		quotes = *(str + i++);
		while (*(str + i) && *(str + i) != quotes)
			i++;
	}
	return (i);
}
