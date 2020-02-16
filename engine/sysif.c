bye_c: // BYE
	asm("bye:");
	*sp = tos;
	manage_io(IO_RESET);
	return sp;

xio_c: // !IO
	manage_io(IO_INIT);
	NEXT();

keyq_c: // KEY?
	PUSH(sp) = tos;
	tos = rx_char();
	if (tos < 0) {
		tos = 0;
	} else {
		PUSH(sp) = tos;
		tos = ~0;
	}
	NEXT();

emit_c: // EMIT
	tx_char(tos);
	tos = POP(sp);
	NEXT();
