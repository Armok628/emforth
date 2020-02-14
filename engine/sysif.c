bye_c:
	asm("bye:");
	*sp = tos;
	return sp;
