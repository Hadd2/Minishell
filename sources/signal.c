/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 21:02:08 by habernar          #+#    #+#             */
/*   Updated: 2024/09/23 19:43:26 by habernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_main(int signal)
{
	g_sigint = signal;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	rl_done = 1;
}

int	sigint_heredoc(t_shell *shell, t_cmd *cmd, char *buffer, int fd)
{
	if (buffer)
		free(buffer);
	shell->exit_code = 130;
	cmd->error = 1;
	g_sigint = 0;
	close(fd);
	return (0);
}

void	setup_signal(void)
{
	signal(SIGINT, sigint_main);
	signal(SIGQUIT, SIG_IGN);
}
