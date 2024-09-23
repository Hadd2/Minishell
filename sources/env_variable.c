/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:45:47 by habernar          #+#    #+#             */
/*   Updated: 2024/09/23 20:28:39 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*erase_alpha(char *str, char *dsign)
{
	char	*s;
	int		i;
	int		size;

	i = 1;
	while (*(dsign + i) && ft_isalnum(*(dsign + i)))
		i++;
	size = ft_strlen(str) - i + 1;
	s = (char *)malloc(sizeof(char) * size);
	if (!s)
		return (0);
	*s = 0;
	ft_strlcat(s, str, dsign - str + 1);
	ft_strlcat(s, dsign + i, size);
	return (s);
}

static char	*expand_alpha(t_shell *shell, char *str, char *dsign)
{
	char	*s;
	char	*key;
	int		i;
	t_item	*item;

	i = 1;
	while (*(dsign + i) && ft_isalnum(*(dsign + i)))
		i++;
	key = ft_strndup(dsign + 1, i - 1);
	if (!key)
		return (0);
	item = hashtable_search(shell->ht, key);
	free(key);
	if (!item || !item->value)
		return (erase_alpha(str, dsign));
	s = (char *)malloc((ft_strlen(str) - i + ft_strlen(item->value) + 1));
	if (!s)
		return (0);
	*s = 0;
	ft_strlcat(s, str, dsign - str + 1);
	ft_strlcat(s, item->value, ft_strlen(s) + ft_strlen(item->value) + 1);
	ft_strlcat(s, dsign + i, ft_strlen(s) + ft_strlen(dsign + i) + 1);
	return (s);
}

static char	*expand_exit_value(t_shell *shell, char *str, char *dsign)
{
	char	*s;
	char	*tmp;
	int		size;

	tmp = ft_itoa(shell->exit_code);
	size = ft_strlen(str) - 2 + ft_strlen(tmp) + 1;
	s = (char *)malloc(sizeof(char) * size);
	if (!s)
	{
		perror("malloc");
		return (0);
	}
	*s = 0;
	ft_strlcat(s, str, dsign - str + 1);
	ft_strlcat(s, tmp, size);
	ft_strlcat(s, dsign + 2, size);
	free(tmp);
	return (s);
}

static char	*expanded_string(t_shell *shell, char *str, char *dsign)
{
	if (*(dsign + 1) && *(dsign + 1) == '?')
		return (expand_exit_value(shell, str, dsign));
	else if (*(dsign + 1) && ft_isalnum(*(dsign + 1)))
		return (expand_alpha(shell, str, dsign));
	else
		return (0);
}

void	expand_env_variables(t_shell *shell, char **tab)
{
	char	*new;
	char	*dollarsign;
	int		i;

	i = 1;
	while (tab && tab[i])
	{
		dollarsign = ft_strchr(tab[i], '$');
		if (dollarsign)
		{
			if (tab[1][0] != '\'')
			{
				new = expanded_string(shell, tab[i], dollarsign);
				printf("%s\n", tab[i]);
				printf("%s\n", new);
				if (new)
				{
					free(tab[i]);
					tab[i] = new;
					continue ;
				}
			}
		}
		i++;
	}
}
