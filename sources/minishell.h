#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>
# include <stdbool.h>
# include <readline/readline.h>
# include "../includes/libft.h"
# define ASCII_SPACE ' '
# define TMP_FILENAME "ASJU43fs8a8i@#98jsa"
# define HT_MAX_LOAD 0.75

# ifdef DEBUG
    #define DEBUG_LOG(fmt, ...) printf("DEBUG: " fmt "\n", ##__VA_ARGS__)
# else
    #define DEBUG_LOG(fmt, ...) // Ne rien faire si DEBUG n'est pas défini
# endif

typedef enum e_file
{
	HEREDOC,
	REDIRIN,
	REDIROUT,
	REDIRAPPEND
}	t_file;

typedef enum e_nodetype
{
	LOGICAL_AND,
	LOGICAL_OR,
	PIPE,
	CMD
}	t_nodetype;

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
	bool	heredoc;
	bool	redirin;
	bool	redirout;
	bool	redirappend;
	int		fdin;
	int		fdout;
	char	*infile;
	char 	*outfile;
	char	*delimiter;
	char	*path;
	char	**params;
}	t_cmd;

typedef struct s_astnode
{
	int					type;
	int					pipein;
	int					pipeout;
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
t_astnode	*ast_make_node(t_shell *shell,int type, t_astnode *left, t_astnode *right);
t_astnode	*ast_make_cmd(t_shell *shell, char *s);
void		find_right_leftmost(t_astnode *n, int fd);
void		find_all_leaf_left(t_astnode *n, int fd);

/* parse */
t_astnode	*parse_logical(t_shell *shell, char **str);

/* parse utils */
bool		nothing_to_parse(char *str, char c);
int			is_whitespace(char c);
int			is_delimiter(char c);
int			skip_whitespace(char **str);
void		skip_quotes(char **str);

/* env variable */
void		expand_env_variables(t_shell *shell, char **tab);

/* string */
char		*ft_strndup(char *str, int len);
char		*ft_strjoin_slash(char *s1, char *s2, char sep);
int			only_capital_letter(char *str);
void		remove_whitespace(char **str);

/*	interpret */
void		execute_cmd(t_shell *shell, t_astnode *n);
void		execute_pipe(t_shell *shell, t_astnode *n);
void		ast_interpret(t_shell *shell, t_astnode *n);

/* command */
t_cmd		*make_command(t_shell *shell, t_astnode *n, char *str);

/* redir */
void		get_redirs(t_cmd *cmd, char *str);
char		*remove_redirs(char *str);
int			skip_whitespace(char **str);

/* free */
void		free_tab(char **tab);
void 		free_cmd(t_cmd *cmd);
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

/* debug */
void 		print_ast(t_astnode *n);
#endif
