here_code: /*: HERE ( here ) ;*/
	ASMLABEL(here_code);
	PUSH(sp) = tos;
	tos = (cell_t)dp;
	NEXT();

allot_code: /*: ALLOT ( allot ) ;*/
	ASMLABEL(allot_code);
	dp = (cell_t *)((char *)dp + tos);
	tos = POP(sp);
	NEXT();

comma_code: /*: , ( comma ) ;*/
	ASMLABEL(comma_code);
	*dp = tos;
	dp++;
	tos = POP(sp);
	NEXT();

c_comma_code: /*: C, ( c_comma ) ;*/
	ASMLABEL(c_comma_code);
	*(char *)dp = (char)tos;
	dp = (cell_t *)((char *)dp+1);
	tos = POP(sp);
	NEXT();
