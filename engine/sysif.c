bye_c: // BYE
	asm("bye:");
	*sp = tos;
	manage_io(IO_RESET);
	return sp;

key_c: // KEY
	asm("key:");
	PUSH(sp) = tos;
	tos = rx_char();
	NEXT();

poll_c: // POLL
	asm("poll:");
	PUSH(sp) = tos;
	tos = poll_rx() ? ~0 : 0;
	NEXT();

emit_c: // EMIT
	asm("emit:");
	tx_char(tos);
	tos = POP(sp);
	NEXT();
