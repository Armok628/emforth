	ASMLABEL(to_r_code);
to_r_code: /*: >R ( to_r ) ;*/
	PUSH(rp) = tos;
	tos = POP(sp);
	NEXT();

	ASMLABEL(r_from_code);
r_from_code: /*: R> ( r_from ) ;*/
	PUSH(sp) = tos;
	tos = POP(rp);
	NEXT();

	ASMLABEL(r_fetch_code);
r_fetch_code: /*: R@ ( r_fetch ) ;*/
	PUSH(sp) = tos;
	tos = rp[-1];
	NEXT();

	ASMLABEL(two_to_r_code);
two_to_r_code: /*: 2>R ( two_to_r ) ;*/
	PUSH(rp) = POP(sp);
	PUSH(rp) = tos;
	tos = POP(sp);
	NEXT();

	ASMLABEL(two_r_from_code);
two_r_from_code: /*: 2R> ( two_r_from ) ;*/
	PUSH(sp) = tos;
	tos = POP(rp);
	PUSH(sp) = POP(rp);
	NEXT();

	ASMLABEL(two_r_fetch_code);
two_r_fetch_code: /*: 2R@ ( two_r_fetch ) ;*/
	PUSH(sp) = tos;
	tos = rp[-1];
	PUSH(sp) = rp[-2];
	NEXT();
