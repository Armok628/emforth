	ASMLABEL(two_dup_code);
two_dup_code: /*: 2DUP ( two_dup ) ;*/
	PUSH(sp) = tos;
	tos = sp[-2];
	PUSH(sp) = tos;
	tos = sp[-2];
	NEXT();

	ASMLABEL(two_drop_code);
two_drop_code: /*: 2DROP ( two_drop ) ;*/
	sp -= 2;
	NEXT();

	ASMLABEL(two_swap_code);
two_swap_code: /*: 2SWAP ( two_swap ) ;*/
	SWAP(cell_t, sp[-3], sp[-1]);
	SWAP(cell_t, sp[-2], tos);
	NEXT();

	ASMLABEL(two_over_code);
two_over_code: /*: 2OVER ( two_over ) ;*/
	PUSH(sp) = tos;
	tos = sp[-4];
	PUSH(sp) = tos;
	tos = sp[-4];
	NEXT();
