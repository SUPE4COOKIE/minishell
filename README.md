# Here is our Minishell !

## Description
This project is a simple shell that can execute commands and manage environment variables. It is a simple version of a real shell.

## Features
- Run commands with their parameters
- Manage environment variables
- Manage the return value of the last command
- Manage signals (Ctrl-C, Ctrl-D, Ctrl-\\)
- Manage redirections (>, <, >>)
- Manage pipes (|)
- Manage environment variables ($ followed by characters)
- Manage exit status ($?)
- Manage the PATH variable
- Manage the builtins: echo, cd, pwd, export, unset, env, exit

## How to use
1. Clone the repository
```bash
git clone https://github.com/scrumier/minishell.git
```
2. Compile the project
```bash
make
```
3. Run the program
```bash
./minishell
```

## Testing with valgrind
```bash
valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes --suppressions=test.supp ./minishell
```

## Authors
- [**scrumier**]
- [**mwojtasi**]
