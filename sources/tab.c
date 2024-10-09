/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 22:12:21 by habernar          #+#    #+#             */
/*   Updated: 2024/10/09 19:35:26 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tab_size(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

void	tab_append(char ***tab, t_buffer *buffer)
{
	int		i;
	char	**new;

	i = 0;
	while (*tab && (*tab)[i])
		i++;
	new = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new)
		return ;
	i = 0;
	while (*tab && (*tab)[i])
	{
		new[i] = (*tab)[i];
		i++;
	}
	new[i++] = strdup(buffer->s);
	new[i] = 0;
	free(*tab);
	*tab = new;
}

bool	tab_contains(char **tab, char *str)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		if (ft_strncmp(tab[i], str, ft_strlen(str)) == 0)
			return (true);
		i++;
	}
	return (false);
}
