#include "minishell.h"

static void	previous_redir(t_cmd *cmd, int type)
{
	if (type == HEREDOC || type == REDIRIN)
	{
		if (cmd->redirin)
			free(cmd->infile);
		if (cmd->heredoc)
			free(cmd->delimiter);
		cmd->delimiter = 0;
		cmd->infile = 0;
		cmd->heredoc = FALSE;
		cmd->redirin = FALSE;
	}
	else if (type == REDIRAPPEND || type == REDIROUT)
	{
		if (cmd->redirout)
			free(cmd->outfile);
		if (cmd->redirappend)
			free(cmd->outfile);
		cmd->outfile = 0;
		cmd->redirappend = FALSE;
		cmd->redirout = FALSE;
	}
}

static void	get_redir_type(t_cmd *cmd, int type, char *str)
{
	previous_redir(cmd, type);
	if (type == HEREDOC)
	{
		cmd->heredoc = TRUE;
		cmd->delimiter = str;
	}
	else if (type == REDIRIN)
	{
		cmd->redirin = TRUE;
		cmd->infile = str;
	}
	else if (type == REDIRAPPEND)
	{
		cmd->redirappend = TRUE;
		cmd->outfile = str;
	}
	else if (type == REDIROUT)
	{
		cmd->redirout = TRUE;
		cmd->outfile = str;
	}
}

static int	get_redir_name(t_cmd *cmd, char *str, int type)
{
	int		size;
	int		whitespace;
	char	*name;

	size = 0;
	whitespace = 0;
	while (*str && (*str == ' ' || *str == '<' || *str == '>'))
	{
		whitespace++;
		str++;
	}
	while (*(str + size) && !is_delimiter(*(str + size)) && !is_whitespace(*(str + size)) && *(str + size) != '<' && *(str + size) != '>')
		size++;
	name = (char *)malloc(sizeof(char) * (size + 1));
	if (!name)
	{
		printf("Error: malloc failed on line %d in file %s\n", __LINE__, __FILE__);
		exit(1);
	}
	ft_memcpy(name, str, size);
	*(name + size) = 0;
	get_redir_type(cmd, type, name);
	return (size + whitespace);
}

/* TODO 
 * only last redir <<eof <infile cat produces heredoc script but only read from infile
 * ex <infile <<eof cat only read from heredoc
 * cmd1 >>output >outfile writes to outfile
 * cmd1 >outfile >>output writes to ouput
 */
void	get_redirs(t_cmd *cmd, char *str)
{
	while (*str)
	{
		if (*str == '<')
		{
			if (*(str + 1) && *(str + 1) == '<')
				str += get_redir_name(cmd, str, HEREDOC) + 2;
			else
				str += get_redir_name(cmd, str, REDIRIN) + 1;
		}
		else if (*str == '>')
		{
			if (*(str + 1) && *(str + 1) == '>')
				str += get_redir_name(cmd, str, REDIRAPPEND) + 2;
			else
				str += get_redir_name(cmd, str, REDIROUT) + 1;
		}
		else
			str++;
	}
}

char	*remove_redirs(char *str)
{
	int		i;
	int		head;

	i = 0;
	while (*(str + i))
	{
		if (*(str + i) == '<' || *(str + i) == '>')
		{
			head = i++;
			if (!ft_strncmp(str + i - 1, "<<", 2) || !ft_strncmp(str + i - 1, ">>", 2))
				i++;
            while (*(str + i) && *(str + i) == ' ')
                i++;
            while (*(str + i) && (*(str + i) != ' ' && *(str + i) != '(' && *(str + i) != ')'))
                i++;
			ft_memmove(str + head, str + i, ft_strlen(str + i) + 1);
			i = head;
		}
		else
			i++;
	}
	printf("rmredir: %s\n", str);
	return (str);
}
