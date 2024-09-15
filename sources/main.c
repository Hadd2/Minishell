/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 22:43:29 by habernar          #+#    #+#             */
/*   Updated: 2024/09/15 22:43:30 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_shell(t_shell *shell, char **env)
{
	int		i;
	char	*k;
	char	*v;

	shell->exit_code = 0;
	shell->parse_error = 0;
	shell->cl = 0;
	shell->headcl = 0;
	shell->ast = 0;
	shell->ht = hashtable_init();
	shell->env = env;
	i = 0;
	while (shell->ht && env && env[i])
	{
        k = ft_strndup(env[i], ft_strchr(env[i], '=') - env[i]);
        v = ft_strdup(ft_strchr(env[i], '=') + 1);
        hashtable_insert(shell->ht, k, v);
/*
#ifdef DEBUG
		DEBUG_LOG("Inserted env var: %s=%s", k, v);
#endif
*/
#ifdef DEBUG
		DEBUG_LOG("Inserted env var: %s=%s", k, v);
#endif
        i++;
    }
}

static int	parsable(char *str)
{
	int		ascii[127];
	int		i;
	char	c;

	if (!str || (*str && *str == '\n' && !*(str + 1)))
		return (0);
	i = 0;
	while (i < 127)
		ascii[i++] = 0;
	while (*str)
	{
		ascii[(int)*str]++;
		if (*str == '\'' || *str == '\"')
		{
			c = *str++;
			while (*str && *str != c)
				str++;
			if (*str == c)
				ascii[(int)*str++]++;
		}
		else 
			str++;
	}

#ifdef DEBUG
	DEBUG_LOG("Parsing input: ASCII counts: \" %d, ' %d, ( %d, ) %d", ascii['\"'], ascii['\''], ascii['('], ascii[')']);
#endif
	return (!ascii['\\'] && !ascii[';'] && ascii['\''] % 2 == 0 && ascii['\"'] % 2 == 0 && ascii['('] == ascii[')']);
}

int main(int argc, char **argv, char **env)
{
	t_shell shell;

	(void)argc, (void)argv;
	init_shell(&shell, env);
	//shell.cl = readline("$");
	//char str[200];
	//size_t size = 200;
	shell.cl = get_next_line(STDIN_FILENO);
	while (shell.cl)
	{
		//printf("%s> ", getcwd(str,	size));
		//shell.cl = get_next_line(STDIN_FILENO);
		shell.parse_error = 0;
		shell.headcl = shell.cl;
		if (!parsable(shell.cl))
		{
			free(shell.headcl);
			shell.headcl = 0;
			shell.cl = get_next_line(STDIN_FILENO);
			continue;
		}

#ifdef DEBUG	
        print_ast(shell.ast);
#endif
		shell.ast = parse_logical(&shell, &shell.cl);
        print_ast(shell.ast);
		if (shell.parse_error == 0)
			ast_interpret(&shell, shell.ast);
		free(shell.headcl);
		free_ast(shell.ast);
		shell.ast = 0;
		shell.headcl = 0;
		shell.cl = get_next_line(STDIN_FILENO);
	}
	hashtable_free(shell.ht);
	//exit_shell(&shell);
	return (0);
}
/*
int main(int argc, char **argv, char **env)
{
    t_shell shell;
    char prompt[256];
    
    (void)argc;
    (void)argv;
    init_shell(&shell, env);

    while (1)
    {
        // Afficher le prompt
        if (getcwd(prompt, sizeof(prompt)) != NULL)
            printf("%s$ ", prompt);
        else
            printf("$ ");  // Fallback si getcwd échoue
        
        // Lire l'entrée utilisateur
        shell.cl = readline(0);
        if (shell.cl == NULL) // Gestion de EOF (Ctrl+D)
            break;

        // Traiter la commande si elle n'est pas vide
        if (*shell.cl != '\0')
        {
            shell.parse_error = 0;
            shell.headcl = shell.cl;

            if (parsable(shell.cl))
            {
                shell.ast = parse_logical(&shell, &shell.cl);
#ifdef DEBUG    
                print_ast(shell.ast);
#endif
                if (shell.parse_error == 0)
                    ast_interpret(&shell, shell.ast);
                
                free_ast(shell.ast);
                shell.ast = NULL;
            }
        }
        shell.headcl = NULL;
    }
	hashtable_free(shell.ht);
    //exit_shell(&shell);
	exit(shell.exit_code);
}
*/
/* TODO SHELL STRUCTURE
 * SIGNAL
 * ENV + ENVVARIABLE
 * HANDLE "" AND '' AND $
 * FINISH EXEC
 * BUILTIN
 * HISTORY
 */
