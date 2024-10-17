# Gyshell

Gyshell is a custom shell implementation written in C by Siyam Al Shahriar<a href="2"/>.

## Features

- Custom prompt displaying the current working directory
- Built-in commands: cd, pwd, echo, env, setenv, exit
- Support for background processes
- Signal handling for Ctrl+C (SIGINT)
- Timeout for foreground processes

## Built-in Commands

1. `cd`: Change the current directory
2. `pwd`: Print the current working directory
3. `echo`: Print arguments to the console, with support for environment variables
4. `env`: Display all environment variables or a specific one
5. `setenv`: Set an environment variable
6. `exit`: Exit the shell

## Usage

To run a command in the background, add `&` at the end of the command.

## Implementation Details

- Uses `fork()` and `execvp()` for executing external commands
- Implements signal handling for SIGINT
- Tokenizes input using `strtok()`
- Manages environment variables using `getenv()` and `setenv()`

## Limitations

- Maximum command line length: 1024 characters
- Maximum number of command line arguments: 128

## Future Improvements

- Implement I/O redirection
- Add support for pipes
- Enhance error handling and user feedback