/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 19:48:48 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:13:35 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static size_t	bytes_needed(int n)
{
	size_t			bytes;
	unsigned int	un;

	bytes = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		un = -n;
		bytes++;
	}
	else
		un = n;
	while (un)
	{
		bytes++;
		un /= 10;
	}
	return (bytes);
}

static char	*reverse(char *s)
{
	size_t	len;
	size_t	i;
	char	c;

	i = 0;
	len = ft_strlen(s);
	while (i < len / 2)
	{
		c = *(s + i);
		*(s + i) = *(s + len - i - 1);
		*(s + len - i - 1) = c;
		i++;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	char			*nbr;	
	char			*head;	
	unsigned int	un;

	nbr = (char *)malloc(sizeof(char) * (bytes_needed(n) + 1));
	if (!nbr)
		return (0);
	head = nbr;
	if (n < 0)
		un = -n;
	else
		un = n;
	if (n == 0)
		*nbr++ = '0';
	while (un)
	{
		*nbr++ = un % 10 + '0';
		un /= 10;
	}
	if (n < 0)
		*nbr++ = '-';
	*nbr = 0;
	return (reverse(head));
}
