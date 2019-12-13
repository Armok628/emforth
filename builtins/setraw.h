#ifndef SETRAW_H
#define SETRAW_H

#ifdef __unix__
#include <unistd.h>
#include <termios.h>
#endif

#include <stdbool.h>

void setraw(bool b) {
#ifdef __unix__
	static bool init = false;
	static struct termios norm, raw;
	if (!init) {
		tcgetattr(STDIN_FILENO, &norm);
		raw = norm;
		cfmakeraw(&raw);
		init = true;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, b ? &raw : &norm);
#else
	(void)b;
	return;
#endif
}

#endif
