two_dup_code: /*: 2DUP ( two_dup ) ;*/
	ASMLABEL(two_dup_code);
	PUSH(sp) = tos;
	tos = sp[-2];
	PUSH(sp) = tos;
	tos = sp[-2];
	NEXT();

two_drop_code: /*: 2DROP ( two_drop ) ;*/
	ASMLABEL(two_drop_code);
	sp--;
	tos = POP(sp);
	NEXT();

two_swap_code: /*: 2SWAP ( two_swap ) ;*/
	ASMLABEL(two_swap_code);
	SWAP(cell_t, sp[-3], sp[-1]);
	SWAP(cell_t, sp[-2], tos);
	NEXT();

two_over_code: /*: 2OVER ( two_over ) ;*/
	ASMLABEL(two_over_code);
	PUSH(sp) = tos;
	tos = sp[-4];
	PUSH(sp) = tos;
	tos = sp[-4];
	NEXT();
