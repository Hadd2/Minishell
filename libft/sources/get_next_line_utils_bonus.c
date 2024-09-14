/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 21:34:10 by habernar          #+#    #+#             */
/*   Updated: 2024/09/09 19:19:41 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

/*
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*head;

	if (!dst || !src)
		return (0);
	head = dst;
	while (n--)
		*(unsigned char *)dst++ = *(unsigned char *)src++;
	return (head);
}
*/

void	array_init(t_array *array)
{
	unsigned int	x;

	array->count = 0;
	array->capacity = BUFFER_SIZE;
	array->buffer = (char *)malloc(sizeof(char) * array->capacity);
	if (!array->buffer)
		array->buffer = 0;
	else
	{
		x = 0;
		while (x < array->capacity)
			*(array->buffer + x++) = 0;
	}
}

void	array_append(t_array *array, char *line)
{
	char			*tmp;
	unsigned int	len;

	len = ft_strlen(line);
	while (array->count + len >= array->capacity)
		array->capacity *= 2;
	tmp = (char *)malloc(sizeof(char) * array->capacity);
	if (!tmp)
		return ;
	ft_memcpy(tmp, array->buffer, array->count);
	free(array->buffer);
	array->buffer = tmp;
	ft_memcpy(array->buffer + array->count, line, len);
	array->count += len;
	*(array->buffer + array->count) = 0;
}

void	*array_delete(t_array *array)
{
	if (array->buffer)
		free(array->buffer);
	array->buffer = 0;
	array->count = 0;
	array->capacity = 0;
	return (0);
}
