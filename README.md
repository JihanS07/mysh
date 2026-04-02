# mysh

A Unix shell implementation written in C from scratch.

## Features

- Command execution via `fork()` and `execvp()`
- Single pipe support (`ls | grep txt`)
- Input redirection (`sort < file.txt`)
- Output redirection (`ls > out.txt`)
- Built-in `cd` command
- `exit` to quit

## Build

```
make
```

Compiles with `-Wall -Werror -Wextra -std=c11`.

## Usage

```
./mysh
```

```
Enter a Command: ls -la
Enter a Command: cat main.c | grep include
Enter a Command: sort < names.txt
Enter a Command: ls > output.txt
Enter a Command: cd src
Enter a Command: exit
```

## How it works

The shell runs a read-eval loop that tokenizes input and checks for operators (`<`, `>`, `|`). Based on what it finds, it configures the appropriate file descriptors with `open()`, `pipe()`, and `dup2()`, then forks child processes to run the commands. The parent waits for all children before prompting again.

`cd` is handled in the parent process directly since forked children can't modify the parent's working directory.
