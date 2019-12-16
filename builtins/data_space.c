	ASMLABEL(here_code);
here_code: /*: HERE ( here ) ;*/
	PUSH(sp) = tos;
	tos = (cell_t)dp;
	NEXT();

	ASMLABEL(allot_code);
allot_code: /*: ALLOT ( allot ) ;*/
	dp = (cell_t *)((char *)dp + tos);
	tos = POP(sp);
	NEXT();

	ASMLABEL(comma_code);
comma_code: /*: , ( comma ) ;*/
	*dp = tos;
	dp++;
	tos = POP(sp);
	NEXT();

	ASMLABEL(c_comma_code);
c_comma_code: /*: C, ( c_comma ) ;*/
	*(char *)dp = (char)tos;
	dp = (cell_t *)((char *)dp+1);
	tos = POP(sp);
	NEXT();
