# VimLike Text Editor

A lightweight terminal text editor written in C using ncurses, inspired by Vim.
Created to practice C and systems programming — but functional enough for real use.

## Features

- Two editing modes: **Normal** and **Insert**
- In-memory line buffer with support for arbitrarily large files
- Vertical scrolling with center-cursor viewport tracking
- Line number gutter
- Undo history
- Open existing files or create new ones

## Build

cd ...//src
make

Requires `ncurses` to be installed (`libncurses-dev` on Debian/Ubuntu, included on macOS).

## Run

cd ...//bin
./text_editor         # open existing file
./text_editor new_file.txt  # create new file

## Keybindings

### Normal mode

| Key      | Action                        |
|----------|-------------------------------|
| `h j k l`| Move cursor left/down/up/right|
| `0`      | Jump to start of line         |
| `$`      | Jump to end of line           |
| `i`      | Enter Insert mode at cursor   |
| `a`      | Enter Insert mode after cursor|
| `o`      | Open new line below           |
| `O`      | Open new line above           |
| `dd`     | Delete current line           |
| `u`      | Undo                          |
| `:w`     | Save file                     |
| `q`      | Quit                          |

### Insert mode

| Key          | Action                        |
|--------------|-------------------------------|
| Arrow keys   | Move cursor                   |
| `Backspace`  | Delete character before cursor|
| `Enter`      | New line                      |
| `Esc`        | Return to Normal mode         |
