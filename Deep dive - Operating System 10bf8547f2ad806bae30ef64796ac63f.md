# Deep dive - Operating System

```bash
sudo strace -c ./siyam/program1  //system call made by program1
```

Above command in Ubuntu will display all the system calls made by a program when it is run to the kernel of the OS.  Below is a list of sys calls made by a program I created.

[Understanding syscall made by my python executable](Understanding%20syscall%20made%20by%20my%20python%20executable%20114f8547f2ad808aab0aef51bcc581b1.md)

There are operating systems for various different architectures. In the personal computer space, let’s contrast between intel and Arm architecture and their OS.

Here are two examples of assembly language source code  for same code written for GNU assembler. Both are compiled assembly source code in `x86_64` (Intel) and `aarch64` (Mac, ARMv8)

```nasm
        .file   "main.c"
        .text
        .globl  main
        .type   main, @function
main:
.LFB0:
        .cfi_startproc
        endbr64
        pushq   %rbp
        .cfi_def_cfa_offset 16
        .cfi_offset 6, -16
        movq    %rsp, %rbp
        .cfi_def_cfa_register 6
        movw    $32322, -14(%rbp)
        movl    $2123412323, -12(%rbp)
        movabsq $431231232323, %rax
        movq    %rax, -8(%rbp)
        movl    $0, %eax
        popq    %rbp
        .cfi_def_cfa 7, 8
        ret
        .cfi_endproc
.LFE0:
        .size   main, .-main
        .ident  "GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
        .section        .note.GNU-stack,"",@progbits
        .section        .note.gnu.property,"a"
        .align 8
        .long   1f - 0f
        .long   4f - 1f
        .long   5
0:
        .string "GNU"
1:
        .align 8
        .long   0xc0000002
        .long   3f - 2f
2:
        .long   0x3
```

```nasm

        .arch armv8-a
        .file   "main.c"
        .text
        .align  2
        .global main
        .type   main, %function
main:
        sub     sp, sp, #16
        mov     w0, 32322
        strh    w0, [sp, 2]
        mov     w0, 45923
        movk    w0, 0x7e90, lsl 16
        str     w0, [sp, 4]
        mov     x0, 26947
        movk    x0, 0x6762, lsl 16
        movk    x0, 0x64, lsl 32
        str     x0, [sp, 8]
        mov     w0, 0
        add     sp, sp, 16
        ret
        .size   main, .-main
        .ident  "GCC: (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.12) 5.4.0 20160609"
        .section        .note.GNU-stack,"",@progbits
```

<aside>
💡

Based on the provided source codes, I can identify the key differences between the two implementations for different architectures:

Architecture: The first code is for x86_64 (Intel) architecture, while the second is for aarch64 (ARM) architecture⁠[1](Deep%20dive%20-%20Operating%20System%2010bf8547f2ad806bae30ef64796ac63f.md)⁠.

Assembly Syntax: The x86_64 code uses AT&T syntax, while the aarch64 code uses ARM assembly syntax.

Register Names: x86_64 uses registers like %rbp, %rsp, and %rax, while aarch64 uses w0, x0, and sp.

Instruction Set: The instructions used are different. For example, x86_64 uses "pushq" and "popq" for stack operations, while aarch64 uses "sub" and "add" with the stack pointer.

Function Prologue/Epilogue: The x86_64 code uses more complex function entry and exit sequences, including frame pointer setup, while the aarch64 code has a simpler prologue and epilogue.

Memory Addressing: x86_64 uses expressions like "-14(%rbp)" for local variable access, while aarch64 uses "[sp, 2]" style addressing.

Immediate Value Handling: aarch64 uses the "movk" instruction for loading large immediate values, which is not present in the x86_64 code.

These differences reflect the distinct design philosophies and capabilities of the two processor architectures.

</aside>

Lets see the differences between Windows and Unix (BSD specifically) to understand the elements that matter in an operating system.

Windows and Unix-based systems (like BSD and other variants) differ in several key ways, including their kernel architectures, system design philosophies, file systems, and user interfaces. Here’s a breakdown of some of the major differences:

### 1. **Kernel Architecture**

- **Windows**:
    - Uses a hybrid kernel (a combination of microkernel and monolithic kernel architectures).
    - It integrates user mode and kernel mode components into a single system, with much of the OS running in kernel mode.
    - Windows kernel is proprietary and closed source, maintained by Microsoft.
- **Unix/BSD**:
    - **BSD** and other Unix variants typically use a monolithic kernel, where most OS services (like file system, process management) run in kernel mode.
    - Unix-based kernels (including BSD) are mostly open-source and follow a simpler, more modular design.
    - **Linux** (a Unix-like OS) also uses a monolithic kernel but allows dynamic loading of modules, giving it flexibility similar to a microkernel.

### 2. **File System and Directory Structure**

- **Windows**:
    - Uses file systems like **NTFS** (New Technology File System), **FAT32**, or **exFAT**.
    - The directory structure starts with a drive letter (`C:\`), and file paths use backslashes (`\`).
    - Follows a less strict file permission model based on NTFS.
- **Unix/BSD**:
    - Common file systems include **UFS** (Unix File System), **ZFS** (in BSD), and **ext4** (in Linux).
    - File paths are hierarchical and rooted at `/`, using forward slashes (`/`).
    - File permissions are stricter and rely on user/group ownership and a comprehensive permissions model (read, write, execute).

### 3. **User Interface**

- **Windows**:
    - Primarily designed for graphical user interface (GUI) use, with tools like Windows Explorer and Control Panel.
    - Although Windows includes a command-line interface (CLI) via **PowerShell** and **Command Prompt**, the GUI is the dominant interface for most users.
- **Unix/BSD**:
    - Traditionally focused on a command-line interface (CLI), with GUIs (like KDE, GNOME, etc.) available on top of it.
    - Users in Unix-like systems often prefer using the terminal for system management, shell scripting (like in **bash** or **zsh**), and automation.
    - BSD systems, in particular, often target more technical users and have minimal default GUI integration compared to Linux distros.

### 4. **Package Management and Software Distribution**

- **Windows**:
    - Software is typically distributed via standalone executable installers (`.exe`, `.msi`).
    - Windows has **Windows Store**, but it's not as central as the command-based package managers in Unix-like systems.
- **Unix/BSD**:
    - Unix systems rely heavily on **package managers** like **pkg** (for BSD), **apt** (Debian-based systems), **yum** or **dnf** (Red Hat-based systems).
    - Software is installed, upgraded, and managed through command-line tools and repositories.

### 5. **Security Model**

- **Windows**:
    - Windows uses **User Account Control (UAC)** for managing administrative privileges.
    - Historically, Windows was more prone to malware and viruses, although recent versions have significantly improved security with tools like **Windows Defender**.
- **Unix/BSD**:
    - Unix-like systems follow the principle of least privilege more strictly, where users must explicitly escalate privileges (usually via **sudo** or root access).
    - Unix and its derivatives are traditionally considered more secure, partly due to their open-source nature and multi-user security model.

### 6. **Networking**

- **Windows**:
    - Windows networking services are often built around proprietary protocols, such as **SMB** for file sharing.
    - Networking tools are managed through graphical interfaces like Network and Sharing Center or command-line tools like **netsh**.
- **Unix/BSD**:
    - Unix systems are built with networking in mind from the start. They support standard networking protocols (like TCP/IP) natively.
    - Networking configurations and services are typically managed through configuration files and CLI tools like `ifconfig`, `ip`, and `netstat`.

### 7. **Licensing**

- **Windows**:
    - Proprietary, commercial software. You need to purchase licenses to use different versions of Windows.
- **Unix/BSD**:
    - **BSD** has permissive open-source licenses, like the **BSD License**, which allows the code to be used and modified with fewer restrictions.
    - Other Unix-like systems, like Linux, are licensed under **GPL** (GNU General Public License), which is more restrictive in ensuring that modifications to the code remain open-source.

### 8. **System Design Philosophy**

- **Windows**:
    - Designed for ease of use, particularly in home and business environments. Emphasizes user-friendly features, GUIs, and integration with a variety of consumer and enterprise software.
- **Unix/BSD**:
    - Unix systems (especially BSD) follow a philosophy of simplicity and modularity, with the focus on small, simple programs that can be combined (via pipes, scripts) to perform complex tasks.

### Conclusion:

Windows and Unix/BSD systems are fundamentally different in their design principles and usage. Windows is designed for broad usability, focusing on ease of access with strong graphical interface integration, while Unix and BSD are more tailored to stability, security, and developer-centric tasks, emphasizing command-line proficiency and open-source flexibility.