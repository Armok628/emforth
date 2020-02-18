bye_c: // BYE
	asm("bye:");
	*sp = tos;
	manage_io(IO_RESET);
	return sp;

keyq_c: // KEY?
	asm("keyq:");
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
	asm("emit:");
	tx_char(tos);
	tos = POP(sp);
	NEXT();
