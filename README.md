# xv6 — System Call Statistics

An extension of the xv6 RISC-V teaching operating system that records system call counts and accumulated elapsed ticks, with a user-space `history` command for inspecting the results.

**C · RISC-V · Operating systems · Kernel instrumentation**

## What this repository adds

- System call instrumentation in [kernel/syscall.c](kernel/syscall.c).
- Per-system-call counters and accumulated timing, protected by spinlocks in [kernel/history.c](kernel/history.c).
- A `history` system call that copies statistics from kernel space to user space.
- A [command-line utility](user/history.c) for viewing all statistics or querying one system call.

## Build and run

Use a Unix-like development environment with GNU Make, a host C compiler, Perl, a RISC-V GCC/binutils toolchain, and `qemu-system-riscv64` available on your PATH.

```sh
git clone https://github.com/satak100/xv6.git
cd xv6
make qemu
```

The [Makefile](Makefile) detects supported RISC-V toolchain prefixes. If detection fails, specify the prefix for your installed toolchain, for example:

```sh
make qemu TOOLPREFIX=riscv64-unknown-elf-
```

## Explore the statistics

At the xv6 shell prompt:

```text
history
history 5
```

`history` lists the available system call statistics. `history 5` queries system call number 5; see [kernel/syscall.h](kernel/syscall.h) for the numeric mapping.

Each line reports the system call name, recorded invocation count, and accumulated elapsed timer ticks.

### How to interpret the results

Timing uses the difference in kernel timer ticks around a system call handler. It measures elapsed time, including time spent blocked, and is not a CPU-time or high-resolution profiler. Calls that complete within one tick can report zero elapsed ticks. Recording happens after the handler returns, so non-returning calls such as `exit` are not counted through this path.

## Source guide

| Path | Purpose |
| --- | --- |
| [kernel/history.c](kernel/history.c) | Statistics storage, locking, and history system call |
| [kernel/syscall.c](kernel/syscall.c) | System call dispatch and timing |
| [kernel/syscall_stat.h](kernel/syscall_stat.h) | Statistics structure shared with user programs |
| [user/history.c](user/history.c) | Statistics viewer |
| [kernel/](kernel/) | Operating system kernel |
| [user/](user/) | User programs and support library |
| [mkfs/](mkfs/) | File-system image builder |

## Attribution and license

This project builds on MIT's xv6 teaching operating system, a reimplementation of Unix Version 6 for RISC-V. See the original [README](README) for upstream acknowledgments and [LICENSE](LICENSE) for the license terms.
