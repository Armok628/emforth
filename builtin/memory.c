store_code: /*: ! ( store ) ;*/
	ASMLABEL(store_code);
	*(cell_t *)tos = POP(sp);
	tos = POP(sp);
	goto next;

fetch_code: /*: @ ( fetch ) ;*/
	ASMLABEL(fetch_code);
	tos = *(cell_t *)tos;
	goto next;

c_store_code: /*: C! ( c_store ) ;*/
	ASMLABEL(c_store_code);
	*(char *)tos = (char)POP(sp);
	tos = POP(sp);
	goto next;

c_fetch_code: /*: C@ ( c_fetch ) ;*/
	ASMLABEL(c_fetch_code);
	tos = *(char *)tos;
	goto next;

addstore_code: /*: +! ( addstore ) ;*/
	ASMLABEL(addstore_code);
	*(cell_t *)tos += POP(sp);
	tos = POP(sp);
	goto next;
