#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <stdbool.h>
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/loader.h"
#include "userprog/gdt.h"
#include "threads/flags.h"
#include "intrinsic.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "userprog/process.h"
void syscall_init (void);

void sys_halt (void);
void sys_exit (int status);
int wait (tid_t * t); 
tid_t sys_fork (const char *thread_name , struct intr_frame *f);
bool sys_create (const char *file, unsigned initial_size);
int sys_open (const char *file);
int sys_filesize (int fd);
int sys_read (int fd, const void *buffer, unsigned size);
int sys_write (int fd, const void *buffer, unsigned size);
void sys_close(int fd);

#endif /* userprog/syscall.h */
