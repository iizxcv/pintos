#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

void write_h(int fd, char* buffer, int size);

#endif /* userprog/syscall.h */
