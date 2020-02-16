docol_c: // DOCOL
	asm("docol:");
	PUSH(rp) = (cell)ip;
	ip = (void ***)&wp[1];
	NEXT();

dolit_c: // DOLIT
	asm("dolit:");
	PUSH(sp) = tos;
	tos = *(cell *)ip++;
	NEXT();
