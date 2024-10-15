/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 20:10:15 by habernar          #+#    #+#             */
/*   Updated: 2024/10/15 20:12:56 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*erase_alpha(char *str, char *dsign)
{
	char	*s;
	int		i;
	int		size;

	i = 1;
	while (*(dsign + i) && (ft_isalnum(*(dsign + i)) || *(dsign + i) == '_'))
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

char	*erase_digit(char *str, char *dsign)
{
	char	*s;
	int		i;
	int		size;

	i = 1;
	while (*(dsign + i) && (ft_isdigit(*(dsign + i))))
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
