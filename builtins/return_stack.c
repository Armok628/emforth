to_r_code: /*: >R ( to_r ) ;*/
	ASMLABEL(to_r_code);
	PUSH(rp) = tos;
	tos = POP(sp);
	next();

r_from_code: /*: R> ( r_from ) ;*/
	ASMLABEL(r_from_code);
	PUSH(sp) = tos;
	tos = POP(rp);
	next();

r_fetch_code: /*: R@ ( r_fetch ) ;*/
	ASMLABEL(r_fetch_code);
	PUSH(sp) = tos;
	tos = rp[-1];
	next();
