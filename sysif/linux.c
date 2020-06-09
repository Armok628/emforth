#include "sysif.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <poll.h>

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
	static struct pollfd in = {
		.fd = STDIN_FILENO,
		.events = POLLIN,
	};
	return poll(&in, 1, 1) > 0;
}

char rx_char(void)
{
	return getchar();
}

void tx_char(char c)
{
	putchar(c);
}
