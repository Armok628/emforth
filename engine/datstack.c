spfetch_c:
	asm("spfetch:");
	PUSH(sp) = tos;
	tos = (cell)sp;
	NEXT();
spstore_c:
	asm("spstore:");
	sp = (cell *)tos;
	tos = POP(sp);
	NEXT();
dup_c:
	asm("dup:");
	PUSH(sp) = tos;
	NEXT();
drop_c:
	asm("drop:");
	tos = POP(sp);
	NEXT();
swap_c:
	asm("swap:");
	SWAP(cell, tos,sp[-1]);
	NEXT();
over_c:
	asm("over:");
	PUSH(sp) = tos;
	tos = sp[-2];
	NEXT();
