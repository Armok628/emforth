#ifndef SETRAW_H
#define SETRAW_H
#include <termios.h>
#include <stdbool.h>

void setraw(bool b) {
	static bool init = false;
	static struct termios norm, raw;
	if (!init) {
		tcgetattr(STDIN_FILENO, &norm);
		raw = norm;
		cfmakeraw(&raw);
		init = true;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, b ? &raw : &norm);
}

#endif
