/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 18:28:54 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:18:41 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	char	*tail_little;
	char	*tail_big;
	size_t	len_little;

	len_little = ft_strlen(little);
	if (!*little)
		return ((char *)big);
	if (!len)
		return (0);
	while (*big && len-- >= len_little)
	{
		tail_big = (char *)big;
		tail_little = (char *)little;
		while (*tail_big++ == *tail_little++)
		{
			if (!(*tail_little))
				return ((char *)big);
		}
		big++;
	}
	return (0);
}
