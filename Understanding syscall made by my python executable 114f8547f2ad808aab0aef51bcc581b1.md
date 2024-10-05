# Understanding syscall made by my python executable

The OCR scheduler program is a executable made using pyinstaller. It is a tkinter GUI based scheduling program with filesystem browsing, thread creation and csv r/w capabilities.

```jsx
strace -c ./main.py
```


```nasm
0.00    0.000000           0        21           read
0.00    0.000000           0         4           write
0.00    0.000000           0         6           open
0.00    0.000000           0        14           close
0.00    0.000000           0         3           fstat
0.00    0.000000           0        10           poll
0.00    0.000000           0         4           lseek
0.00    0.000000           0       104           mmap
0.00    0.000000           0        99           munmap
0.00    0.000000           0         3           brk
0.00    0.000000           0         1           rt_sigprocmask
0.00    0.000000           0         7           ioctl
0.00    0.000000           0         8           readv
0.00    0.000000           0         5         4 access
0.00    0.000000           0         1           dup
0.00    0.000000           0         2           getpid
0.00    0.000000           0         2           socket
0.00    0.000000           0         1           connect
0.00    0.000000           0         4           recvfrom
0.00    0.000000           0         2           shutdown
0.00    0.000000           0         1           bind
0.00    0.000000           0         1           listen
0.00    0.000000           0         1           getsockname
0.00    0.000000           0         1           execve
0.00    0.000000           0         1           uname
0.00    0.000000           0         4           getcwd
0.00    0.000000           0         1         1 readlink
0.00    0.000000           0         1           getppid
0.00    0.000000           0         1           getpgid
0.00    0.000000           0         1           arch_prctl
0.00    0.000000           0         4           gettid
0.00    0.000000           0         1           sched_getaffinity
0.00    0.000000           0         1           set_tid_address
0.00    0.000000           0         4           accept4
0.00    0.000000           0         1           signalfd4

```

## Explanation of each system calls:

1. **`read`**: Reads data from a file descriptor (usually from a file or stdin).
2. **`write`**: Writes data to a file descriptor (stdout, files, etc.).
3. **`open`**: Opens a file or resource for reading/writing.
4. **`close`**: Closes an open file descriptor.
5. **`fstat`**: Gets metadata (like size, permissions) of a file descriptor.
6. **`poll`**: Waits for events on file descriptors (used for I/O operations).
7. **`lseek`**: Repositions the file offset of the open file descriptor (used for random access in files).
8. **`mmap`**: Maps files or devices into memory (often used for memory management or shared memory).
9. **`munmap`**: Unmaps memory that was mapped with `mmap`.
10. **`brk`**: Adjusts the program's data segment (for memory allocation).
11. **`rt_sigprocmask`**: Examines or changes the signal mask, to block or unblock certain signals.
12. **`ioctl`**: Device-specific input/output control operations.
13. **`readv`**: Reads data from multiple buffers (similar to `read` but for multiple buffers).
14. **`access`**: Checks a user's permissions for a file (read/write/execute).
15. **`dup`**: Duplicates a file descriptor.
16. **`getpid`**: Retrieves the process ID of the calling process.
17. **`socket`**: Creates an endpoint for communication (used for networking).
18. **`connect`**: Initiates a connection on a socket.
19. **`recvfrom`**: Receives messages from a socket.
20. **`shutdown`**: Shuts down part or all of a full-duplex connection on a socket.
21. **`bind`**: Associates a socket with a specific address/port.
22. **`listen`**: Marks a socket as passive (for accepting connections).
23. **`getsockname`**: Retrieves the address bound to a socket.
24. **`execve`**: Executes a program (replaces the current process image with a new process).
25. **`uname`**: Returns system information (OS name, kernel version, etc.).
26. **`getcwd`**: Gets the current working directory.
27. **`readlink`**: Reads the value of a symbolic link.
28. **`getppid`**: Retrieves the parent process ID.
29. **`getpgid`**: Retrieves the process group ID.
30. **`arch_prctl`**: Architect-specific process control (used to set architecture-specific settings).
31. **`gettid`**: Retrieves the thread ID.
32. **`sched_getaffinity`**: Gets the CPU affinity of a process.
33. **`set_tid_address`**: Sets the location for storing the child thread ID at thread exit.
34. **`accept4`**: Accepts a connection on a socket (like `accept`, but with additional flags).
35. **`signalfd4`**: Creates a file descriptor that is used to receive signals.

These syscalls are fundamental for handling file I/O, memory management, networking, process control, and interacting with the operating system. Many of them are essential for any Python program running on Linux to manage resources efficiently.
