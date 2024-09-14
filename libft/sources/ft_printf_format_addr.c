/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format_addr.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 18:02:53 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:15:39 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static void	handle_nil_addr(t_token *token)
{
	unsigned int	len;
	unsigned int	size;

	len = ft_strlen(ADDR_NIL);
	size = len;
	if (len < token->width)
		size = token->width;
	if (!token->left)
		ft_putnchar(token, ' ', size - len);
	ft_putnstr(token, ADDR_NIL, len);
	if (token->left)
		ft_putnchar(token, ' ', size - len);
}

void	fill_format_addr(t_token *token, void *addr)
{
	char			*p;
	unsigned int	size;
	unsigned int	len;

	if (!addr)
		return (handle_nil_addr(token));
	p = ft_itoa_unknown_base(token, (uint64_t)addr);
	if (!p)
		return ;
	len = ft_strlen(p);
	size = len + 2;
	if (token->width > size)
		size = token->width;
	if (!token->left)
		ft_putnchar(token, ' ', size - len - SIZEPREFIX);
	ft_putnstr(token, PREFIXLOHEX, SIZEPREFIX);
	ft_putnstr(token, p, ft_strlen(p));
	if (token->left)
		ft_putnchar(token, ' ', size - len - SIZEPREFIX);
	free(p);
}
