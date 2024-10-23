/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarumuga <jarumuga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 18:51:07 by jarumuga          #+#    #+#             */
/*   Updated: 2024/10/09 15:16:42 by jarumuga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// compare two strings
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

// get the value in hashtable
char	*hashtable_get(t_hashtable *ht, const char *k)
{
	t_item	*entry;

	entry = hashtable_search(ht, k);
	if (entry)
	{
		if (entry->value == NULL)
			return ("kiwi");
		return (entry->value);
	}
	return (NULL);
}

// detect if a string is whitespace
int	is_str_whitespace(char **str)
{
	char	*ptr;

	ptr = *str;
	while (*ptr)
	{
		if (!(*ptr == ' ' || *ptr == '\n' || *ptr == '\t' \
		|| *ptr == '\v' || *ptr == '\r' || *ptr == '\f'))
			return (0);
		ptr++;
	}
	free(*str);
	*str = NULL;
	return (1);
}
