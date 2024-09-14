/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format_str.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 23:07:17 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:16:13 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static unsigned int	get_len(t_token *token, char *str)
{
	unsigned int	len;

	len = ft_strlen(str);
	if (token->precision && token->num_precision < len)
	{
		if (ft_strncmp(str, STR_NULL, ft_strlen(str)) == 0)
			len = 0;
		else
			len = token->num_precision;
	}
	return (len);
}

static unsigned int	get_size(t_token *token, unsigned int len)
{
	if (token->width > len)
		return (token->width);
	return (len);
}

void	fill_format_str(t_token *token, char *str)
{
	unsigned int	len;
	unsigned int	size;
	char			*p;

	if (!str)
		p = ft_strdup(STR_NULL);
	else
		p = ft_strdup(str);
	if (!p)
		return ;
	len = get_len(token, p);
	size = get_size(token, len);
	if (!token->left)
		ft_putnchar(token, ' ', size - len);
	ft_putnstr(token, p, len);
	if (token->left)
		ft_putnchar(token, ' ', size - len);
	free(p);
}
