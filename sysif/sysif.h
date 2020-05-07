#ifndef SYSIF_H
#define SYSIF_H
#include <stdbool.h>

enum io_state {
	IO_INIT,
	IO_RESET,
};

void manage_io(enum io_state);
// ^ initializes/resets I/O devices
//   depending on argument

bool poll_rx(void);
// ^ polls input device, returns true if data ready

char rx_char(void);
// ^ blocks, reads one byte from input device

void tx_char(char);
// ^ transmits one to the output device

#endif
