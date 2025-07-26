#include "custom_File.h"

typedef enum
{
	C_STDIN,
	C_STDOUT,
	C_FILE,
	C_DIRECTORY
} file_type;

struct File
{
	struct file *file; // 기본 file 구조체 또는 NULL
	file_type ftype;   // file, stdin, stdout, directory 구분
};

struct File *
File_open(const char *name)
{
	struct file *f = filesys_open(name);
	if (f == NULL)
		return NULL;
	int size = (inode_length(file_get_inode(f)) / PGSIZE) + 1;
	struct File *wrapped = palloc_get_multiple(PAL_ZERO | PAL_ASSERT, size);
	if (wrapped == NULL)
	{
		file_close(f);
		return NULL;
	}

	wrapped->file = f;
	wrapped->ftype = inode_is_dir(file_get_inode(f)) ? C_DIRECTORY : C_FILE;
	return wrapped;
}

int File_read(struct File *f, void *buffer, unsigned size)
{
	if (f == NULL)
		return -1;

	switch (f->ftype)
	{
	case C_STDIN:
		for (unsigned i = 0; i < size; i++)
		{
			((char *)buffer)[i] = input_getc();
		}
		return size;
	case C_FILE:
		return file_read(f->file, buffer, size);
	default:
		return -1;
	}
}

int File_write(struct File *f, const void *buffer, unsigned size)
{
	if (f == NULL)
		return -1;

	switch (f->ftype)
	{
	case C_STDOUT:
		putbuf(buffer, size);
		return size;
	case C_FILE:
		
		return (int)file_write(f->file, buffer, size);
	default:
		return -1;
	}
}
void File_close(struct File* file){
	file_close(file->file);
}


bool File_remove(const char *name)
{
	return filesys_remove(name);
}

off_t
File_length (struct File *file) {
	return file_length (file->file);
}

struct File *
File_open_stdin(void) {
	stdin_file.file = NULL;
	stdin_file.ftype = C_STDIN;
	return &stdin_file;
}

struct File *
File_open_stdout(void) {
	stdin_file.file = NULL;
	stdin_file.ftype = C_STDOUT;
	return &stdin_file;
}



