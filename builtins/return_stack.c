to_r_code: /*: >R ( to_r ) ;*/
	ASMLABEL(to_r_code);
	PUSH(rp) = tos;
	tos = POP(sp);
	NEXT();

r_from_code: /*: R> ( r_from ) ;*/
	ASMLABEL(r_from_code);
	PUSH(sp) = tos;
	tos = POP(rp);
	NEXT();

r_fetch_code: /*: R@ ( r_fetch ) ;*/
	ASMLABEL(r_fetch_code);
	PUSH(sp) = tos;
	tos = rp[-1];
	NEXT();

two_to_r_code: /*: 2>R ( two_to_r ) ;*/
	ASMLABEL(two_to_r_code);
	PUSH(rp) = POP(sp);
	PUSH(rp) = tos;
	tos = POP(sp);
	NEXT();

two_r_from_code: /*: 2R> ( two_r_from ) ;*/
	ASMLABEL(two_r_from_code);
	PUSH(sp) = tos;
	tos = POP(rp);
	PUSH(sp) = POP(rp);
	NEXT();

two_r_fetch_code: /*: 2R@ ( two_r_fetch ) ;*/
	ASMLABEL(two_r_fetch_code);
	PUSH(sp) = tos;
	tos = rp[-1];
	PUSH(sp) = rp[-2];
	NEXT();
