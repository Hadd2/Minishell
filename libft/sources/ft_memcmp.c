/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 18:19:55 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:15:00 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (n-- && *(unsigned char *)s1 == *(unsigned char *)s2)
	{
		s1++;
		s2++;
		if (n == 0)
			return (0);
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
