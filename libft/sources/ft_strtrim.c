/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:15:28 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:18:58 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static int	is_index(char c, char const *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

static size_t	size_trimmed(char const *s1, char const *set)
{
	size_t	count;
	char	*tail;

	count = 0;
	while (is_index(*s1, set))
		s1++;
	while (*s1)
	{
		tail = (char *)s1;
		while (is_index(*tail, set))
		{
			if (!*(tail + 1))
				return (count);
			tail++;
		}
		count++;
		s1++;
	}
	return (count);
}

static char	*trimage(char const *s1, char const *set, char *p_ret)
{
	char	*tail;
	char	*head;

	head = p_ret;
	while (is_index(*s1, set))
		s1++;
	while (*s1)
	{
		tail = (char *)s1;
		while (is_index(*tail, set))
		{
			if (!*(tail + 1))
			{
				*p_ret = 0;
				return (head);
			}
			tail++;
		}
		*p_ret++ = *s1++;
	}
	*p_ret = 0;
	return (head);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ret;

	if (!s1 || !set)
		return (0);
	ret = (char *)malloc(sizeof(char) * (size_trimmed(s1, set) + 1));
	if (!ret)
		return (0);
	return (trimage(s1, set, ret));
}
