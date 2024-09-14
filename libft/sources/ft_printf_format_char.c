/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format_char.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 17:55:01 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:15:47 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	fill_format_char(t_token *token, int c)
{
	unsigned int	size;

	size = 1;
	if (token->width > size)
		size = token->width;
	if (token->type == TYPE_MOD)
		return (ft_putnchar(token, c, 1));
	if (!token->left)
		ft_putnchar(token, ' ', size - 1);
	ft_putnchar(token, c, 1);
	if (token->left)
		ft_putnchar(token, ' ', size - 1);
}
