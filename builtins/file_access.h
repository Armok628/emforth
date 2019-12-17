#ifndef FILE_ACCESS
#define FILE_ACCESS

#ifdef __unix__
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
typedef int fileid;
enum fam {
	RO = O_RDONLY,
	WO = O_WRONLY,
	RW = O_RDWR,
};
#else
#error
#endif

static cell_t ior = 0;

// All of these are inlined because each one should only be used once.
static inline fileid open_file(char *, size_t, enum fam);
static inline fileid create_file(char *, size_t, enum fam);
static inline cell_t close_file(fileid);
static inline ssize_t read_file(void *, size_t, fileid);
static inline ssize_t write_file(void *, size_t, fileid);

#ifdef __unix__
static inline fileid open_file(char *name, size_t len, enum fam acc)
{
	name[len] = '\0'; // ?
	errno = 0;
	fileid fd = open(name, acc);
	ior = errno;
	return fd;
}
static inline fileid create_file(char *name, size_t len, enum fam acc)
{
	name[len] = '\0'; // ?
	errno = 0;
	fileid fd = open(name, O_CREAT | acc);
	ior = errno;
	return fd;
}
static inline cell_t close_file(fileid fd)
{
	errno = 0;
	close(fd);
	return errno;
}
static inline ssize_t read_file(void *buf, size_t count, fileid fd)
{
	errno = 0;
	ssize_t n = read(fd, buf, count);
	ior = errno;
	return n;
}
static inline ssize_t write_file(void *buf, size_t count, fileid fd)
{
	errno = 0;
	ssize_t n = write(fd, buf, count);
	ior = errno;
	return n;
}
#else
#error
#endif

#endif
