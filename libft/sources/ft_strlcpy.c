/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 20:00:37 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:18:12 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	char	*head;	

	if (!dsize)
		return (ft_strlen(src));
	head = (char *)src;
	while (dsize-- > 1 && *src)
	{
		*dst++ = *src++;
	}
	*dst = 0;
	return (ft_strlen(head));
}
