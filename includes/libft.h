/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 18:15:33 by habernar          #+#    #+#             */
/*   Updated: 2024/08/01 20:47:10 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include <stdbool.h>

/* libft */

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(char const *s);
void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
size_t	ft_strlcpy(char *dst, const char *src, size_t dsize);
size_t	ft_strlcat(char *dst, const char *src, size_t dsize);

int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int		ft_atoi(const char *s);

void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);

char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_itoa(int n);

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));

void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new_node);
void	ft_lstadd_back(t_list **lst, t_list *new_node);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/* get_next_line */

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 20
# endif

# ifndef FD_MAX
#  define FD_MAX 1024
# endif

typedef struct s_array
{
	uint64_t	count;
	uint64_t	capacity;
	char		*buffer;
}	t_array;

char	*get_next_line(int fd);
void	array_init(t_array *array);
void	array_append(t_array *array, char *line);
void	*array_delete(t_array *array);

/* printf */

# define PREFIXLOHEX "0x"
# define PREFIXUPHEX "0X"
# define SIZEPREFIX 2
# define BASE_LOHEX "0123456789abcdef"
# define BASE_UPHEX "0123456789ABCDEF"
# define BASE_DECI "0123456789"
# define STR_NULL "(null)"
# define ADDR_NIL "(nil)"

typedef enum e_type
{
	TYPE_CHAR,
	TYPE_STR,
	TYPE_ADDR,
	TYPE_DECI,
	TYPE_INT,
	TYPE_UINT,
	TYPE_LOHEX,
	TYPE_UPHEX,
	TYPE_MOD,
	TYPE_ERROR,
	TYPE_UNINIT
}	t_type;

typedef struct s_token
{
	bool			left;
	bool			zero_pad;
	bool			prefix;
	bool			blank;
	bool			sign;
	bool			precision;
	unsigned int	num_precision;
	unsigned int	width;
	unsigned int	count;
	t_type			type;
}	t_token;

int		ft_printf(const char *buffer, ...);
int		is_flags(char c);
int		is_valid_type(char c);
int		max(unsigned int a, unsigned int b);
void	reset_token(t_token *token);
void	scan_token(t_token *token, const char **buf);
void	parse_token(t_token *token, va_list params);
void	fill_format_uint(t_token *token, unsigned int u);
void	fill_format_str(t_token *token, char *str);
void	fill_format_int(t_token *token, int d);
void	fill_format_char(t_token *token, int c);
void	fill_format_addr(t_token *token, void *addr);
void	ft_putnstr(t_token *token, const char *str, unsigned int len);
void	ft_putnchar(t_token *token, char c, unsigned int n);
char	*ft_itoa_unknown_base(t_token *token, uint64_t num);

#endif
