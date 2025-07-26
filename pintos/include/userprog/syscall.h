#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

void halt_h(void);
void exit_h(int status);
int write_h(int fd, const char *buffer, unsigned size);
int fork_h(const char *thread_name, struct intr_frame *f);
int exec_h(const char *cmd_line);
int wait_h(pid_t pid);
int read_h(int fd, void *buffer, unsigned size);
static void close_h(int fd);
static int filesize_h(int fd);

#endif /* userprog/syscall.h */
