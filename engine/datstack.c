spfetch_c: // SP@
	asm("spfetch:");
	PUSH(sp) = tos;
	tos = (cell)sp;
	NEXT();

spstore_c: // SP!
	asm("spstore:");
	sp = (cell *)tos;
	tos = POP(sp);
	NEXT();

dup_c: // DUP
	asm("dup:");
	PUSH(sp) = tos;
	NEXT();

drop_c: // DROP
	asm("drop:");
	tos = POP(sp);
	NEXT();

swap_c: // SWAP
	asm("swap:");
	SWAP(cell, tos,sp[-1]);
	NEXT();

over_c: // OVER
	asm("over:");
	PUSH(sp) = tos;
	tos = sp[-2];
	NEXT();
