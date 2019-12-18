#ifndef FILE_ACCESS
#define FILE_ACCESS

#include <stdio.h>
#include <errno.h>

typedef FILE *fileid;
enum fam {
	RO = 1<<0,
	WO = 1<<1,
	RW = RO | WO,
	BIN = 1<<2,
	NEW = 1<<3,
};

static cell_t ior = 0;

char *fam2str(enum fam acc)
{
	static char str[4];
	int n = 0;
	if (acc & RO) {
		str[n++] = 'r';
		if (acc & WO)
			str[n++] = '+';
	} else if (acc & WO) {
		str[n++] = 'w';
	}
	if (acc & NEW)
		str[0] = 'w';
	if (acc & BIN)
		str[n++] = 'b';
	str[n] = '\0';
	return str;
}
// All of these are inlined because each one should only be used once.
static inline fileid open_file(char *name, size_t len, enum fam acc)
{
	name[len] = '\0'; // ?
	errno = 0;
	fileid f = fopen(name, fam2str(acc));
	ior = errno;
	return f;
}
static inline fileid create_file(char *name, size_t len, enum fam acc)
{
	return open_file(name, len, acc | NEW);
}
static inline cell_t close_file(fileid f)
{
	return fclose(f) ? errno : 0;
}
static inline int read_file(void *buf, size_t count, fileid f)
{
	clearerr(f);
	ssize_t n = fread(buf, sizeof(char), count, f);
	ior = ferror(f);
	return n;
}
static inline int write_file(void *buf, size_t count, fileid f)
{
	clearerr(f);
	ssize_t n = fwrite(buf, sizeof(char), count, f);
	ior = ferror(f);
	return n;
}

#endif
