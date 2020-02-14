docol_c:
	asm("docol:");
	PUSH(rp) = (cell)ip;
	ip = (void ***)&wp[1];
	NEXT();
dolit_c:
	asm("dolit:");
	PUSH(sp) = tos;
	tos = *(cell *)ip++;
	NEXT();
