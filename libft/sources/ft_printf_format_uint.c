/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format_uint.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 17:59:22 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:16:21 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static unsigned int	get_size(t_token *token, char *p, unsigned int u)
{
	unsigned int	len;

	len = ft_strlen(p);
	if (token->precision && token->num_precision > len)
		len = token->num_precision;
	if (token->prefix && !(token->type == TYPE_UINT) && u != 0)
		len += 2;
	if (token->width > len)
		len = token->width;
	if (token->num_precision == 0 && token->precision
		&& u == 0 && token->width == 0)
		len = 0;
	return (len);
}

static void	left_fill_uint(t_token *token, char *p, unsigned int size)
{
	unsigned int	len;
	unsigned int	max_num_len;

	len = ft_strlen(p);
	max_num_len = max(token->num_precision, len);
	if (token->prefix && token->type != TYPE_UINT && *p != '0')
	{
		if (token->type == TYPE_LOHEX)
			ft_putnstr(token, PREFIXLOHEX, SIZEPREFIX);
		if (token->type == TYPE_UPHEX)
			ft_putnstr(token, PREFIXUPHEX, SIZEPREFIX);
	}
	if (token->precision && token->num_precision > len)
	{
		ft_putnchar(token, '0', token->num_precision - len);
	}
	ft_putnstr(token, p, len);
	if (token->prefix && token->type != TYPE_UINT && *p != '0')
		ft_putnchar(token, ' ', size - max_num_len - SIZEPREFIX);
	else
		ft_putnchar(token, ' ', size - max_num_len);
}

static void	handle_zero_pad(t_token *token, char *p, unsigned int size)
{
	unsigned int	len;

	len = ft_strlen(p);
	if (token->prefix && token->type != TYPE_UINT)
	{
		if (token->type == TYPE_LOHEX)
			ft_putnstr(token, PREFIXLOHEX, SIZEPREFIX);
		if (token->type == TYPE_UPHEX)
			ft_putnstr(token, PREFIXUPHEX, SIZEPREFIX);
		ft_putnchar(token, '0', size - SIZEPREFIX - len);
	}
	else
		ft_putnchar(token, '0', size - len);
}

static void	right_fill_uint(t_token *token, char *p, unsigned int size)
{
	unsigned int	len;
	unsigned int	max_num_len;

	len = ft_strlen(p);
	max_num_len = max(token->num_precision, len);
	if (token->zero_pad && !token->precision)
		handle_zero_pad(token, p, size);
	else
	{
		if (token->width > len + SIZEPREFIX && token->prefix)
			ft_putnchar(token, ' ', size - max_num_len - SIZEPREFIX);
		else if (token->width > len)
			ft_putnchar(token, ' ', size - max_num_len);
		if (token->prefix && token->type != TYPE_UINT && *p != '0')
		{
			if (token->type == TYPE_LOHEX)
				ft_putnstr(token, PREFIXLOHEX, SIZEPREFIX);
			if (token->type == TYPE_UPHEX)
				ft_putnstr(token, PREFIXUPHEX, SIZEPREFIX);
		}
		if (token->precision && token->num_precision > len)
			ft_putnchar(token, '0', token->num_precision - len);
	}
	ft_putnstr(token, p, len);
}

void	fill_format_uint(t_token *token, unsigned int u)
{
	char			*p;
	unsigned int	size;

	p = ft_itoa_unknown_base(token, u);
	if (!p)
		return ;
	size = get_size(token, p, u);
	if (token->num_precision == 0 && token->precision && u == 0)
	{
		ft_putnchar(token, ' ', size);
		return (free(p));
	}
	if (token->left)
		left_fill_uint(token, p, size);
	else
		right_fill_uint(token, p, size);
	free(p);
}
