# 🐚 Minishell

A beautiful, minimalistic reimplementation of the Unix shell in C, following the strict guidelines of the 42 school curriculum. This project recreates essential features of Bash, including parsing, execution, built-in commands, redirections, pipes, and environment handling, while respecting the complexities of UNIX signal management and memory hygiene.

## 🚀 Features

### ✅ Core Requirements

- Custom shell prompt with interactive input using `readline`
- Command execution via:
  - Absolute and relative paths
  - `$PATH` lookup
- Quoting support:
  - `'single quotes'` — literal interpretation
  - `"double quotes"` — expansion of `$VARIABLES`
- Environment variable expansion (`$VAR`, `$?`)
- Proper handling of:
  - `Ctrl-C` — interrupts and redraws prompt
  - `Ctrl-D` — exits shell
  - `Ctrl-\` — ignored

### 🔁 Built-in Commands

- `echo [-n]` — print arguments
- `cd [dir]` — change working directory
- `pwd` — print current directory
- `export [VAR=VALUE]` — add/update environment variable
- `unset [VAR]` — remove environment variable
- `env` — print environment
- `exit` — exit shell

### 🔧 Redirections

- `<` — redirect input
- `>` — redirect output (overwrite)
- `>>` — redirect output (append)
- `<<` — heredoc (input until delimiter)

### 🧪 Execution & Pipes

- Pipes (`|`) for chaining commands
- Proper child process handling
- Exit code tracking with `$?`

### 🧠 Bonus Features

- `&&` and `||` logical operators with `()` for priority
- Wildcard `*` expansion (glob) in current directory

## ⚙️ Build & Run

### Prerequisites

- Unix-like environment
- GCC compiler
- `readline` library

### Build

```bash
make
```

### Run

```bash
./minishell
```

## 🧼 Makefile Targets

- `make` / `make all` — builds the project
- `make clean` — removes object files
- `make fclean` — removes all generated files
- `make re` — rebuilds everything from scratch

## 🧠 Project Constraints

- **Only one** global variable allowed (signal handling only)
- Memory leaks must be avoided (except from `readline`)
- External functions limited to an approved list
- Follow Bash behavior wherever applicable

## 📜 License

This project is part of the **42 School Curriculum** and adheres to their academic integrity rules. Sharing or using this code outside the intended scope is discouraged.
