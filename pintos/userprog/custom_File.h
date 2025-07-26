#include "filesys/filesys.h"
#include "filesys/file.h"
#include "threads/palloc.h"
#include "threads/vaddr.h"
#include <debug.h>
#include <stdio.h>
#include <string.h>

static struct File stdin_file;
static struct File stdout_file;

enum file_type;

struct File;

struct File *File_open(const char *name);
int File_read(struct File *f, void *buffer, unsigned size);
int File_write(struct File *f, const void *buffer, unsigned size);
void File_close(struct File* file);
bool File_remove(const char *name);
File_length (struct File *file);
File_open_stdin(void);
File_open_stdout(void);
