#include "sysif.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>

void manage_io(enum io_state s)
{
	static bool init = false;
	static struct termios old, new;
	if (!init) {
		tcgetattr(STDIN_FILENO, &old);
		tcgetattr(STDIN_FILENO, &new);
		cfmakeraw(&new);
		init = true;
	}
	switch (s) {
	case IO_INIT:
		tcsetattr(STDIN_FILENO, TCSANOW, &new);
		break;
	case IO_RESET:
		tcsetattr(STDIN_FILENO, TCSANOW, &old);
		break;
	}
}

bool poll_rx(void)
{
	fd_set set;
	struct timeval tv;

	FD_ZERO(&set);
	FD_SET(STDIN_FILENO, &set);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	return select(STDIN_FILENO + 1, &set, NULL, NULL, &tv);
}

char rx_char(void)
{
	return getchar();
}

void tx_char(char c)
{
	putchar(c);
}
