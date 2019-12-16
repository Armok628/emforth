	ASMLABEL(store_code);
store_code: /*: ! ( store ) ;*/
	*(cell_t *)tos = POP(sp);
	tos = POP(sp);
	NEXT();

	ASMLABEL(fetch_code);
fetch_code: /*: @ ( fetch ) ;*/
	tos = *(cell_t *)tos;
	NEXT();

	ASMLABEL(c_store_code);
c_store_code: /*: C! ( c_store ) ;*/
	*(char *)tos = (char)POP(sp);
	tos = POP(sp);
	NEXT();

	ASMLABEL(c_fetch_code);
c_fetch_code: /*: C@ ( c_fetch ) ;*/
	tos = *(char *)tos;
	NEXT();

	ASMLABEL(addstore_code);
addstore_code: /*: +! ( addstore ) ;*/
	*(cell_t *)tos += POP(sp);
	tos = POP(sp);
	NEXT();

// : 2! ( two_store ) TUCK ! CELL+ ! ;
// : 2@ ( two_fetch ) DUP CELL+ @ SWAP @ ;
