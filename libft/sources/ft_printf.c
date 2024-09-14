/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:18:12 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:15:33 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

int	ft_printf(const char *buffer, ...)
{
	int		count;	
	va_list	params;
	t_token	token;

	count = 0;
	if (!buffer)
		return (-1);
	va_start(params, buffer);
	while (*buffer)
	{
		reset_token(&token);
		if (*buffer == '%')
		{
			buffer++;
			scan_token(&token, &buffer);
			parse_token(&token, params);
			count += token.count;
		}
		else
			count += write(1, buffer++, 1);
	}
	va_end(params);
	return (count);
}
