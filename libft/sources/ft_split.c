/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:30:53 by habernar          #+#    #+#             */
/*   Updated: 2024/09/14 22:02:19 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	flag;

	count = 0;
	flag = 1;
	while (*s)
	{
		if (*s == c)
			flag = 1;
		else if (*s != c && flag == 1)
		{
			flag = 0;
			count++;
		}
		s++;
	}
	return (count);
}

static int	ft_strndup(char **tab, char const *s, int len)
{
	char	*ret;
	char	*head;

	if (len == 0)
		return (0);
	ret = (char *)malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (0);
	head = ret;
	while (*s && len--)
		*ret++ = *s++;
	*ret = 0;
	*tab = head;
	return (1);
}

static char	**free_tab(char **res)
{
	int	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		i++;
	}
	free(res);
	return (0);
}

static void	init_tab(char **ret, char const *s, int c)
{
	int	len_tab;
	int	idx;

	idx = 0;
	len_tab = count_words(s, c);
	while (idx <= len_tab)
		ret[idx++] = 0;
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		len;
	int		idx;

	if (!s)
		return (0);
	res = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!res)
		return (0);
	idx = 0;
	init_tab(res, s, c);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		len = 0;
		while (*s && *s != c)
		{
			len++;
			s++;
		}
		if (len && !ft_strndup(&res[idx++], s - len, len))
			return (free_tab(res));
	}
	return (res[idx] = 0, res);
}
