#include "userprog/syscall.h"static 
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/loader.h"
#include "userprog/gdt.h"
#include "threads/flags.h"
#include "intrinsic.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "userprog/process.h"
#include "user/syscall.h"
#include "threads/vaddr.h" // 필요 시 유효 주소 검증에 사용
#include "custom_File.h"

void syscall_entry(void);
void syscall_handler(struct intr_frame *);

// syscall handler helpers
void halt_h(void);
void exit_h(int status);
int write_h(int fd, const char *buffer, unsigned size);
int fork_h(const char *thread_name, struct intr_frame *f);
int exec_h(const char *cmd_line);
int wait_h(pid_t pid);
int read_h(int fd, void *buffer, unsigned size);
static void close_h(int fd);
static int filesize_h(int fd);

static void syscall_handler(struct intr_frame *f)
{
	int syscall_num = f->R.rax;

	switch (syscall_num)
	{
	case SYS_HALT:
		halt_h();
		break;

	case SYS_EXIT:
		exit_h((int)f->R.rdi);
		break;

	case SYS_WRITE:
		f->R.rax = write_h((int)f->R.rdi, (const char *)f->R.rsi, (unsigned)f->R.rdx);
		break;

	case SYS_FORK:
		f->R.rax = fork_h((const char *)f->R.rdi, f);
		break;

	case SYS_EXEC:
		f->R.rax = exec_h((const char *)f->R.rdi);
		break;

	case SYS_WAIT:
		f->R.rax = wait_h((pid_t)f->R.rdi);
		break;

	case SYS_READ:
		f->R.rax = read_h((int)f->R.rdi, (void *)f->R.rsi, (unsigned)f->R.rdx);
		break;

	case SYS_OPEN:
		f->R.rax = open_h((const char *)f->R.rdi);
		break;

	case SYS_CLOSE:
		close_h((int)f->R.rdi);
		break;

	default:
		printf("Unknown system call: %d\n", syscall_num);
		thread_exit();
	}
}

static void halt_h(void)
{
	power_off(); // pintos 기본 함수
}

static void exit_h(int status)
{
	struct thread *cur = thread_current();
	printf("%s: exit(%d)\n", cur->name, status);
	thread_exit(); // 실제로 종료 처리
}

static int write_h(int fd, const char *buffer, unsigned size)
{
	struct thread *cur_t = thread_current();
	struct File * f = cur_t->fdt[fd];
	if (buffer == NULL){return -1;} 
	return File_write(fd, buffer,size);

}

static int fork_h(const char *thread_name, struct intr_frame *f)
{
	return process_fork(thread_name, f); // pintos 기본 제공
}

static int exec_h(const char *cmd_line)
{
	if (cmd_line == NULL)
		return -1;
	return process_exec(cmd_line); // 실행 실패하면 -1
}

int wait_h(pid_t pid)
{
	return process_wait(pid);
}

/* Read system call */
int read_h(int fd, void *buffer, unsigned size) {
	struct thread *cur_t = thread_current();
	if (buffer == NULL){return -1;} 
	return File_read(cur_t->fdt[fd],buffer,size);

}

/* Open system call */
static int open_h(const char *file) {

	struct thread *cur_t = thread_current();
	struct File * f = File_open(file);
	if(f == NULL){return -1;}
	cur_t->fdt[cur_t->fdt_cur_cnt] = f;
	return cur_t->fdt_cur_cnt++; 
}

/* Close system call */
static void close_h(int fd) {
	struct thread *cur_t = thread_current();
	if(cur_t->fdt[fd] == NULL){return exit(-1);}
	struct File *f =cur_t->fdt[fd];
	File_close(f);

	// fd로 열린 파일 찾아서 file_close()
	// 열린 파일 테이블에서 제거
}

/* filesize system call */
static int filesize_h(int fd) {
	struct thread *cur_t = thread_current();
	if(cur_t->fdt[fd] == NULL){return;}
	struct File *f =cur_t->fdt[fd];
	
	return File_length(f);
	// file_length
	// fd로 열린 파일 찾아서 file_close()
	// 열린 파일 테이블에서 제거
}