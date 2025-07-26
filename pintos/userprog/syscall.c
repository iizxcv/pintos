#include "userprog/syscall.h"
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

void syscall_entry(void);
void syscall_handler(struct intr_frame *);

// syscall handler helpers
void halt_h(void);
void exit_h(int status);
int write_h(int fd, const char *buffer, unsigned size);
int fork_h(const char *thread_name, struct intr_frame *f);
int exec_h(const char *cmd_line);
int wait_h(pid_t pid);

void syscall_handler(struct intr_frame *f)
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

void halt_h(void)
{
	power_off(); // pintos 기본 함수
}

void exit_h(int status)
{
	struct thread *cur = thread_current();
	printf("%s: exit(%d)\n", cur->name, status);
	thread_exit(); // 실제로 종료 처리
}

int write_h(int fd, const char *buffer, unsigned size)
{
	if (fd == 1)
	{
		putbuf(buffer, size);
		return size;
	}
	return -1; // fd != 1은 아직 미구현
}

int fork_h(const char *thread_name, struct intr_frame *f)
{
	return process_fork(thread_name, f); // pintos 기본 제공
}

int exec_h(const char *cmd_line)
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
	int w_size = 0;
	if (fd == 0) {
		char *buf = (char *)buffer;

		for (int i = 0; i < size; i++){
			char key = input_getc();
			buf[i] = key;
			w_size++;
		}
			buf[w_size++] = '\0';

		// 표준 입력(stdin)
		// 키보드 입력은 실제 구현시 input_getc() 등을 반복 호출
		// 여기선 간단히 비워둠
		// ex: ((char *)buffer)[i] = input_getc();
		return w_size; // 임시로 0 바이트 읽었다고 가정
	}
	// 파일에서 읽는 경우: file descriptor lookup → file_read()
	return -1; // 아직 미구현
}

/* Open system call */
int open_h(const char *file) {
	if (file == NULL)
		return -1;

	// filesys_open(file) → struct file*
	// open file list에 추가하고 fd 할당 필요
	return -1; // 미구현
}

/* Close system call */
void close_h(int fd) {
	// fd로 열린 파일 찾아서 file_close()
	// 열린 파일 테이블에서 제거
}