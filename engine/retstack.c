rpfetch_c:
	asm("rpfetch:");
	PUSH(sp) = tos;
	tos = (cell)rp;
	NEXT();
rpstore_c:
	asm("rpstore:");
	rp = (cell *)tos;
	tos = POP(sp);
	NEXT();
to_r_c:
	asm("to_r:");
	PUSH(rp) = tos;
	tos = POP(sp);
	NEXT();
r_fetch_c:
	asm("r_fetch:");
	PUSH(sp) = tos;
	tos = rp[-1];
	NEXT();
r_from_c:
	asm("r_from:");
	PUSH(sp) = tos;
	tos = POP(rp);
	NEXT();
