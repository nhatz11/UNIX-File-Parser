# UNIX File Parser

## Description

**UNIX File Parser** is a minimal C utility that reads and parses the UNIX/Linux `/etc/passwd` file using only low-level system calls (`open`, `read`, and `write`). It demonstrates direct file I/O, buffer management, and basic string parsing without using the C standard library (`stdio.h` functions).

This project demonstrates:
- Raw system call usage for file operations
- Manual memory management and line parsing in C
- In-place string tokenization for fast field extraction

## What It Does

For every line in `/etc/passwd`, the parser extracts and prints:
- The **username** (field 1)
- The **shell path** (field 7)

### Example

Given this line from `/etc/passwd`:

ubuntu:x:1001:1001:,,,:/home/ubuntu:/bin/bash

The program would output:

ubuntu:/bin/bash


## How It Works

1. **File Reading:** I used the `read` system call to pull the file into a buffer in chunks.
2. **Manual Line Splitting:** I wrote a loop that copies characters to a `line_buffer` and detects when a newline occurs.
3. **In-place Parsing:** Instead of allocating new memory for every field, I replaced the colons (`:`) with null characters (`\0`) directly in the buffer.
4. **Output:** Once I isolated the 1st and 7th fields, I printed them using `write`.

### Key Concepts

- **No Standard Library:** I avoided `fopen`, `fgets`, or `printf` to keep the overhead low.
- **Direct System Calls:** I wanted to interact directly with the kernel via file descriptors.
- **Buffer Safety:** I included checks to ensure my buffers don't overflow when reading long lines.

## Why I Built This

I wanted to challenge myself to parse text files without the "magic" of high-level libraries. This project helped me get comfortable with the kind of memory management and pointer arithmetic required in OS and kernel-level development.

## Usage

**Build:**

gcc passwd_parser.c -o passwd_parser

**Run:**

./passwd_parser

(Note: This requires read permission for `/etc/passwd`.)

## Example Output

root:/bin/bash
daemon:/usr/sbin/nologin
ubuntu:/bin/bash
...
