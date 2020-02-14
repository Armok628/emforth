store_c:
	asm("store:");
	*(cell *)tos = POP(sp);
	tos = POP(sp);
	NEXT();
fetch_c:
	asm("fetch:");
	tos = *(cell *)tos;
	NEXT();
cstore_c:
	asm("cstore:");
	*(char *)tos = POP(sp);
	tos = POP(sp);
	NEXT();
cfetch_c:
	asm("cfetch:");
	tos = *(char *)tos;
	NEXT();
