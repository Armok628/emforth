#include "sysif.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>

void manage_io(enum io_state s)
{
	static bool init = false;
	static struct termios old, new;
	if (!init) {
		tcgetattr(STDIN_FILENO, &old);
		new = old;
		new.c_lflag &= ~(ICANON|ECHO);
		new.c_cc[VMIN] = 0;
		new.c_cc[VTIME] = 0;
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

int rx_char(void)
{
	char c;
	if (read(STDIN_FILENO, &c, 1) > 0)
		return c;
	else
		return -1;
}

void tx_char(int c)
{
	write(STDOUT_FILENO, &c, 1);
}
