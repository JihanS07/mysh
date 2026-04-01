# mysh

A shell written in C from scratch. Built to understand how shells actually work under the hood: process creation, pipes, file descriptors, all of it.

## What it does

- Reads commands and runs them using `fork()` and `execvp()`
- Supports piping between two commands (like `ls | grep txt`)
- Handles `cd` as a builtin (since child processes can't change the parent's directory)
- `exit` to quit

## Building

```
make
```

This compiles with `-Wall -Werror -Wextra` because strict warnings keep you honest.

## Running

```
make run
```

or just

```
./mysh
```

## How it works

The main loop reads input, tokenizes it, and checks for pipes. If there's a pipe, it forks two children and connects them with `pipe()` and `dup2()`. Otherwise it forks once and execs the command. The parent waits for children to finish before prompting again.

## Status

Still a work in progress. More features coming as I keep learning.
