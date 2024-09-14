/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_numbers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 00:18:35 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:16:27 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static unsigned int	bytes_needed(uint64_t u, unsigned int len_base)
{
	unsigned int	size;

	size = 0;
	if (u == 0)
		return (1);
	while (u)
	{
		u /= len_base;
		size++;
	}
	return (size);
}

static char	*reverse(char *str)
{
	unsigned int	len;	
	unsigned int	i;	
	char			x;

	i = 0;
	len = ft_strlen(str);
	while (i < len / 2)
	{
		x = *(str + i);
		*(str + i) = *(str + len - i - 1);
		*(str + len - i - 1) = x;
		i++;
	}
	return (str);
}

static char	*ft_itoa_base(uint64_t n, const char *base)
{
	unsigned int	len_b;
	char			*ret;
	char			*head;

	len_b = ft_strlen(base);
	ret = (char *)malloc(sizeof(char) * (bytes_needed(n, len_b) + 1));
	if (!ret)
		return (0);
	head = ret;
	if (n == 0)
		*ret++ = '0';
	while (n)
	{
		*ret++ = base[n % len_b];
		n /= len_b;
	}
	*ret = 0;
	return (reverse(head));
}

char	*ft_itoa_unknown_base(t_token *token, uint64_t num)
{
	if (token->type == TYPE_LOHEX || token->type == TYPE_ADDR)
		return (ft_itoa_base(num, BASE_LOHEX));
	else if (token->type == TYPE_UPHEX)
		return (ft_itoa_base(num, BASE_UPHEX));
	return (ft_itoa_base(num, BASE_DECI));
}
