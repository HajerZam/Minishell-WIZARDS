# 🧙‍♂️ Minishell-Wizard

> "A wizard is never late, nor is he early — he executes precisely when he means to."  
> — *Some POSIX-compliant Gandalf*

<div align="center">

![Minishell Banner](https://media.giphy.com/media/v1.Y2lkPTc5MGI3NjExYmQ1ZGRkMmFkNzllZTAyNzE2YzdlODFkYzZjY2JjMThmNzljZmY2MyZjdD1n/Nx0rz3jtxtEre/giphy.gif)

![Language](https://img.shields.io/badge/language-C-00599C?style=flat-square&logo=c)
![School](https://img.shields.io/badge/42-Network-000000?style=flat-square&logo=42)
![Build](https://img.shields.io/badge/build-success-brightgreen?style=flat-square)
![Shell](https://img.shields.io/badge/shell-POSIX%20Compliant-yellowgreen?style=flat-square)
![License](https://img.shields.io/badge/license-42%20Internal-blueviolet?style=flat-square)

</div>

---

## 🧾 Overview

**Minishell-Wizard** is a handcrafted mini shell, built from scratch in C as part of the 42 School curriculum. It interprets, parses, and executes command-line instructions with support for redirections, pipelines, environment variables, and built-in functions.

🪄 No `system()` calls.  
🪄 No shortcuts.  
🪄 Just pure system-level wizardry.

---

## 🗂️ Features

- 🔹 **Custom prompt** with `readline()`
- 🔹 Lexical analysis (quotes, escapes, operators)
- 🔹 Parser that builds command structures
- 🔹 Pipes `|` and redirections `< > >> <<`
- 🔹 Heredoc support
- 🔹 Full environment variable support (`$VAR`, `$?`)
- 🔹 Built-in commands: `cd`, `echo`, `pwd`, `env`, `exit`, `export`, `unset`
- 🔹 Signal handling (`SIGINT`, `SIGQUIT`)
- 🔹 Memory-safe & Valgrind-clean

---

## 🏗️ Project Structure

```bash
Minishell-Wizard/
├── lexer/        # Tokenization logic
├── parser/       # Syntax tree & command parsing
├── executor/     # Fork, execve, piping, redirs
├── builtins/     # Built-in command implementations
├── env/          # Environment variable support
├── signals/      # Signal handling
├── utils/        # Helper functions
├── main.c        # Program entry point
├── minishell.h   # Global headers
└── Makefile      # Compilation logic
```

---

## ⚙️ Built-ins
Command	Description
echo	Print strings
cd	Change directory
pwd	Print current path
export	Set environment variable
unset	Remove environment variable
env	Show current environment
exit	Exit the shell

## 🧪 Usage
bash
Copy
Edit
# Clone and compile
git clone https://github.com/yourusername/Minishell-Wizard.git
cd Minishell-Wizard
make

## Run the wizard shell
./minishell
❗ readline is required. Install with:
brew install readline (Mac) or sudo apt-get install libreadline-dev (Linux)

## 🧠 Example Commands

```bash
Copy
Edit
$ echo Hello Wizard
Hello Wizard

$ export USERNAME=Gandalf
$ echo "Welcome, $USERNAME"
Welcome, Gandalf

$ ls | grep .c > output.txt

$ cat << EOF
This is a heredoc
With multiple lines!
EOF
```

---

## 🧙 Authors
👤 Wizard A – @HajerZam

👤 Wizard B – @FedePenna

With patience, GDB, and a few invocations of valgrind.

---

## 📜 License
This project is part of the 42 school curriculum. Redistribution is governed by 42’s internal rules and honor code.

---

## 🌈 Bonus Ideas (If you dare...)

- 🔥 Wildcard/globbing support (*, ?)
- 🧾 Command history
- 🧠 Syntax highlighting
- 🧼 Autoformatting output

---

## 🐚 Job control (&, fg, bg) — high level

<div align="center">
  ✨ Made with love and pain at 42 Network
</div>
