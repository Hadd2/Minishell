/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format_int.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 18:00:59 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:16:04 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static unsigned int	get_size(t_token *token, char *p, int d)
{
	unsigned int	size;

	size = ft_strlen(p);
	if (token->precision && token->num_precision > size)
		size = token->num_precision;
	if (token->blank || token->sign || d < 0)
		size += 1;
	if (token->width > size)
		size = token->width;
	if (d == 0 && token->precision
		&& token->num_precision == 0 && token->width == 0)
		size = 0;
	return (size);
}

static void	handle_sign(t_token *token, int d)
{
	if (token->sign || token->blank || d < 0)
	{
		if (d < 0)
			ft_putnchar(token, '-', 1);
		else
		{
			if (token->blank)
				ft_putnchar(token, ' ', 1);
			else
				ft_putnchar(token, '+', 1);
		}
	}
}

static void	left_fill_int(t_token *token, char *p, int d, unsigned int size)
{
	unsigned int	len;
	int				sign;

	sign = 0;
	if (token->sign || token->blank || d < 0)
		sign = 1;
	len = ft_strlen(p);
	handle_sign(token, d);
	if (token->precision && token->num_precision > len)
		ft_putnchar(token, '0', token->num_precision - len);
	ft_putnstr(token, p, len);
	ft_putnchar(token, ' ', size - max(token->num_precision, len) - sign);
}

static void	right_fill_int(t_token *token, char *p, int d, unsigned int size)
{
	unsigned int	len;
	int				f;

	f = 0;
	len = ft_strlen(p);
	if (token->sign || token->blank || d < 0)
		f = 1;
	if (size > len + f)
	{
		if (token->zero_pad && !token->precision)
		{
			handle_sign(token, d);
			ft_putnchar(token, '0', size - len - f);
		}
		else
		{
			ft_putnchar(token, ' ', size - max(token->num_precision, len) - f);
			handle_sign(token, d);
			if (token->precision && token->num_precision > len)
				ft_putnchar(token, '0', token->num_precision - len);
		}
	}
	else
		handle_sign(token, d);
	ft_putnstr(token, p, len);
}

void	fill_format_int(t_token *token, int d)
{
	char			*p;
	unsigned int	size;

	p = ft_itoa(d);
	if (!p)
		return ;
	if (*p == '-')
		ft_memmove(p, p + 1, ft_strlen(p));
	size = get_size(token, p, d);
	if (token->num_precision == 0 && token->precision && d == 0)
	{
		ft_putnchar(token, ' ', size);
		return (free(p));
	}
	if (token->left)
		left_fill_int(token, p, d, size);
	else
		right_fill_int(token, p, d, size);
	free(p);
}
