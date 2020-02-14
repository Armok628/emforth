bye_c: // BYE
	asm("bye:");
	*sp = tos;
	return sp;
