/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_scanner.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 16:19:49 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 21:16:39 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

static void	scan_flags(t_token *token, const char **buf)
{
	while (is_flags(**buf))
	{
		if (**buf == '0')
			token->zero_pad = 1;
		else if (**buf == '#')
			token->prefix = 1;
		else if (**buf == ' ')
			token->blank = 1;
		else if (**buf == '-')
			token->left = 1;
		else if (**buf == '+')
			token->sign = 1;
		(*buf)++;
	}
}

static void	scan_width(t_token *token, const char **buf)
{
	int	result;

	result = 0;
	while (ft_isdigit(**buf))
	{
		result = 10 * result + **buf - '0';
		(*buf)++;
	}
	token->width = result;
}

static void	scan_precision(t_token *token, const char **buf)
{
	int	result;

	result = 0;
	if (**buf != '.')
		return ;
	token->precision = 1;
	(*buf)++;
	while (ft_isdigit(**buf))
	{
		result = 10 * result + **buf - '0';
		(*buf)++;
	}
	token->num_precision = result;
}

static void	scan_type(t_token *token, const char **buf)
{
	if (**buf == 'c')
		token->type = TYPE_CHAR;
	else if (**buf == 's')
		token->type = TYPE_STR;
	else if (**buf == 'p')
		token->type = TYPE_ADDR;
	else if (**buf == 'd')
		token->type = TYPE_DECI;
	else if (**buf == 'i')
		token->type = TYPE_INT;
	else if (**buf == 'u')
		token->type = TYPE_UINT;
	else if (**buf == 'x')
		token->type = TYPE_LOHEX;
	else if (**buf == 'X')
		token->type = TYPE_UPHEX;
	else if (**buf == '%')
		token->type = TYPE_MOD;
	else
		token->type = TYPE_ERROR;
	(*buf)++;
}

void	scan_token(t_token *token, const char **buf)
{
	scan_flags(token, buf);
	scan_width(token, buf);
	scan_precision(token, buf);
	scan_type(token, buf);
}
