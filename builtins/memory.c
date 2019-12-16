store_code: /*: ! ( store ) ;*/
	ASMLABEL(store_code);
	*(cell_t *)tos = POP(sp);
	tos = POP(sp);
	NEXT();

fetch_code: /*: @ ( fetch ) ;*/
	ASMLABEL(fetch_code);
	tos = *(cell_t *)tos;
	NEXT();

c_store_code: /*: C! ( c_store ) ;*/
	ASMLABEL(c_store_code);
	*(char *)tos = (char)POP(sp);
	tos = POP(sp);
	NEXT();

c_fetch_code: /*: C@ ( c_fetch ) ;*/
	ASMLABEL(c_fetch_code);
	tos = *(char *)tos;
	NEXT();

addstore_code: /*: +! ( addstore ) ;*/
	ASMLABEL(addstore_code);
	*(cell_t *)tos += POP(sp);
	tos = POP(sp);
	NEXT();

// : 2! ( two_store ) TUCK ! CELL+ ! ;
// : 2@ ( two_fetch ) DUP CELL+ @ SWAP @ ;
