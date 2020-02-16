#ifndef SYSIF_H
#define SYSIF_H

enum io_state {
	IO_INIT,
	IO_RESET,
};

void manage_io(enum io_state);
// ^ initializes/resets I/O devices
//   depending on argument

int rx_char(void);
// ^ polls input device for a character
//     returns <0 if no key ready
//     otherwise returns key value

void tx_char(int);
// ^ transmits character to output device

#endif
