/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 22:48:09 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:15:16 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	void	*head;

	if (!dest && !src)
		return (0);
	head = dest;
	if (dest < src)
	{
		while (n--)
			*(unsigned char *)dest++ = *(unsigned char *)src++;
	}
	else
	{
		while (n--)
		{
			*(unsigned char *)(dest + n) = *(unsigned char *)(src + n);
		}
	}
	return (head);
}
