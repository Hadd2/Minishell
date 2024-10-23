# MiniShell

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Signal Handling](#signal-handling)
- [Error Handling](#error-handling)

## Overview

MiniShell is a lightweight, minimalist shell created as part of a coding project. It aims to replicate some core functionality of larger shells like `bash`, providing a simple command-line interface where users can execute basic commands, manage processes, handle file I/O, and work with pipes and redirection.

## Features

- Command execution with path resolution (similar to `/bin/sh`).
- Basic built-in commands (e.g., `cd`, `echo`, `exit`, etc.).
- Support for environment variables.
- Input/output redirection (`>`, `<`, `>>`).
- Piping support (`|`) to connect multiple commands.
- Signal handling for interrupting processes.
- Error handling with meaningful messages.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/Hadd2/Minishell.git
   cd minishell
   ```

2. Build the project using `make`:
   ```bash
   make
   ```

3. Run the MiniShell:
   ```bash
   ./minishell
   ```

## Usage

Once you have started the MiniShell, you can execute commands similarly to how you would in a regular shell.

### Basic Usage Examples:
- Execute a simple command:
  ```bash
  minishell> ls -l
  ```

- Change directory:
  ```bash
  minishell> cd /path/to/directory
  ```

- Use pipes:
  ```bash
  minishell> ls | grep minishell
  ```

- Redirect output to a file:
  ```bash
  minishell> echo "Hello, World!" > output.txt
  ```

- Append to a file:
  ```bash
  minishell> echo "New line" >> output.txt
  ```

- Exit the shell:
  ```bash
  minishell> exit
  ```

## Built-in Commands

MiniShell supports the following built-in commands:

| Command | Description                              |
|---------|------------------------------------------|
| `cd`    | Change the current directory             |
| `echo`  | Print text to the terminal               |
| `pwd`   | Print the current working directory      |
| `export`| Set an environment variable              |
| `unset` | Remove an environment variable           |
| `exit`  | Exit the MiniShell                       |

Commands not built into MiniShell are executed by searching the system's `$PATH`.

## Signal Handling

MiniShell handles common signals such as `SIGINT` (Ctrl+C) to interrupt running processes, and `SIGQUIT` (Ctrl+\) for quitting processes. It ensures the shell itself isn't terminated by these signals, only the foreground processes.

## Error Handling

MiniShell provides error messages for:
- Invalid command inputs.
- Misuse of file redirections or pipes.
- Directory changes when the path is invalid.
- Permission errors when trying to execute commands or access files.

Error messages are printed to `stderr` in a clear and informative manner.
