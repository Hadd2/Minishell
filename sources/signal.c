/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: habernar <habernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 21:02:08 by habernar          #+#    #+#             */
/*   Updated: 2024/10/13 20:57:08 by habernar         ###   ########.fr       */
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
	g_sigint = 0;
	signal(SIGINT, sigint_main);
	signal(SIGQUIT, SIG_IGN);
}

void	wait_command(t_shell *shell, t_cmd *cmd)
{
	int	status;

	if (wait(&status) == cmd->pid)
	{
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			g_sigint = 0;
			shell->exit_code = 130;
		}
		else if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		else
			shell->exit_code = 128 + WTERMSIG(status);
	}
	if (shell->exit_code == 131)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
	setup_signal();
}
