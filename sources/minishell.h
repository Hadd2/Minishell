/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:42:35 by habernar          #+#    #+#             */
/*   Updated: 2024/10/13 20:57:39 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <string.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <stdint.h>
# include <dirent.h>
# include <sys/stat.h>
# include "../includes/libft.h"
# define ASCII_SPACE ' '
# define TMP_FILENAME "ASJU43fs8a8i@#98jsa"
# define PATH "usr/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"
# define ENV "/usr/bin/env"
# define MSG_ERROR_FILEORDIR "minishell : %s : no such file or directory\n"
# define HT_MAX_LOAD 0.75
# define ALPHSIZE 255
# define HEREDOC_EOF "minishell: warming: <here-document>\
	found end of file instead of \"%s\"\n"

extern volatile sig_atomic_t	g_sigint;

typedef enum e_ftype
{
	HEREDOC,
	REDIRIN,
	REDIROUT,
	REDIRAPPEND
}	t_ftype;

typedef struct s_file
{
	t_ftype	type;
	char	*name;
}	t_file;

typedef enum e_toktype
{
	T_UNINIT = -1,
	T_AND,
	T_OR,
	T_PIPE,
	T_DSUP,
	T_DINF,
	T_SSUP,
	T_SINF,
	T_LEFTP,
	T_RIGHTP,
	T_ALNUM,
	T_EOF
}	t_toktype;

typedef struct s_tok
{
	t_toktype	type;
}	t_tok;

typedef enum e_nodetype
{
	LOGICAL_AND,
	LOGICAL_OR,
	PIPE,
	CMD,
	BRACKET
}	t_nodetype;

typedef struct s_buffer
{
	char	s[256];
	int		i;
}	t_buffer;

typedef struct s_trie
{
	struct s_trie	*children[ALPHSIZE];
	bool			isword;
}	t_trie;

typedef struct s_item
{
	char	*key;
	char	*value;
	bool	tombstone;
}	t_item;

typedef struct s_hashtable
{
	int		size;
	int		count;
	t_item	**items;
}	t_hashtable;

typedef struct s_cmd
{
	int		pid;
	int		exitcode;
	int		error;
	char	*path;
	char	**params;
	t_list	*lstfiles;
}	t_cmd;

typedef struct s_astnode
{
	int					type;
	char				*ps;
	t_cmd				*cmd;
	struct s_astnode	*left;
	struct s_astnode	*right;
}	t_astnode;

typedef struct s_shell
{
	int			exit_code;
	int			parse_error;
	char		*cl;
	char		*headcl;
	char		**env;
	t_hashtable	*ht;
	t_astnode	*ast;
}	t_shell;

/* ast */
t_astnode	*ast_make_node(t_shell *s, int type, t_astnode *l, t_astnode *r);
t_astnode	*ast_make_cmd(t_shell *shell, char *s);
void		find_right_leftmost(t_astnode *n, int fd);
void		find_all_leaf_left(t_astnode *n, int fd);
void		erase_right_leftmost(t_astnode **n, int count);
void		pipe_left_subtree(t_astnode *n, int fd);
void		pipe_right_subtree(t_astnode *n, int fd);

/* parse */
t_astnode	*parse_logical(t_shell *shell, char **str);

/* parse utils */
bool		nothing_to_parse(char *str);
int			is_whitespace(char c);
int			is_delimiter(char c);
int			skip_whitespace(char **str);
void		skip_quotes(char **str);

/* env variable */
char		*expand_env_variables(t_shell *shell, char *str);

/* string */
char		*ft_strndup(char *str, int len);
char		*ft_strjoin_slash(char *s1, char *s2, char sep);
int			only_capital_letter(char *str);
void		remove_whitespace(char **str);
int			in_single_quotes(char *str, char *c);

/*	interpret */
void		ast_interpret(t_shell *shell, t_astnode *n);

/* command */
void		make_command(t_shell *shell, t_astnode *n);

/* redir */
void		get_redirs(t_shell *shell, t_cmd *cmd, char *str);
char		*remove_redirs(char *str);
int			skip_whitespace(char **str);

/* init */
void		init_shell(t_shell *shell, int argc, char **argv, char **env);

/* free */
void		free_tab(char **tab);
void		free_cmd(t_cmd *cmd);
void		free_ast(t_astnode *n);
void		exit_shell(t_shell *shell);

/* hashtable */
t_item		*item_create(char *k, char *v);
void		item_delete(t_item *item);
t_hashtable	*hashtable_init(void);
void		hashtable_free(t_hashtable *ht);
uint32_t	hash(const char *key);
void		hashtable_delete(t_hashtable *ht, const char *k);
t_item		*hashtable_search(t_hashtable *ht, const char *k);
void		hashtable_resize(t_hashtable *ht);
bool		hashtable_insert(t_hashtable *ht, char *k, char *v);

/* file */
int			get_here_doc(t_shell *shell, t_cmd *cmd, char *delimiter);
void		handle_fd(t_astnode *n);

/* signal */
void		setup_signal(void);
int			sigint_heredoc(t_shell *shell, t_cmd *cmd, char *buffer, int fd);
void		wait_command(t_shell *shell, t_cmd *cmd);

/* trie */
t_trie		*trie_create(void);
void		trie_insert(t_trie **root, char *str);
bool		trie_search(t_trie *node, char *target);
void		trie_free(t_trie *node);
void		search_files(t_trie **root);

/* wildcard */
void		expand_wildcard(t_cmd *cmd);

/* tab */
int			tab_size(char **tab);
void		tab_append(char ***tab, t_buffer *buffer);
bool		tab_contains(char **tab, char *str);

/* quotes */
void		remove_quotes(char **tab);
void		expand_quotes(char **tab);
int			ignore_quotes(char *str);

/* token */
bool		check_token(char *str);

/* token utils */
bool		ft_isalnumsup(char c);
bool		is_operator(t_toktype type);
bool		is_redirection(t_toktype type);

/* debug */
void		print_ast(t_astnode *n);
void		print_cmd(t_cmd *cmd);

#endif
