#ifndef INC_SYSCALLS_H_
#define INC_SYSCALLS_H_

#include <stddef.h>
#include <sys/stat.h>
#include <sys/times.h>

// Semihosting initialization
void initialise_monitor_handles(void);

// Syscall prototypes
void _exit(int status);
int _close(int file);
int _execve(char *name, char **argv, char **env);
int _fork(void);
int _fstat(int file, struct stat *st);
int _getpid(void);
int _isatty(int file);
int _kill(int pid, int sig);
int _link(char *old, char *new);
int _lseek(int file, int ptr, int dir);
int _open(char *path, int flags, ...);
int _read(int file, char *ptr, int len);
int _stat(char *file, struct stat *st);
struct tms;  // Forward declaration for tms struct
int _times(struct tms *buf);
int _unlink(char *name);
int _wait(int *status);
int _write(int file, char *ptr, int len);
// The prototype for _sbrk, used for malloc
void *_sbrk(ptrdiff_t incr);

// Prototypes for standard I/O retargeting functions.
// These functions are implemented elsewhere (e.g., main.c)
// and marked as 'weak' so they can be optionally provided.
int __io_putchar(int ch);
int __io_getchar(void);

#endif /* INC_SYSCALLS_H_ */