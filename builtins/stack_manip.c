	ASMLABEL(lit_code);
lit_code: /*: LIT ( lit ) ;*/
	PUSH(sp) = tos;
	tos = *(cell_t *)ip++;
	NEXT();

	ASMLABEL(dup_code);
dup_code: /*: DUP ( dup ) ;*/
	PUSH(sp) = tos;
	NEXT();

	ASMLABEL(drop_code);
drop_code: /*: DROP ( drop ) ;*/
	tos = POP(sp);
	NEXT();

	ASMLABEL(swap_code);
swap_code: /*: SWAP ( swap ) ;*/
	SWAP(cell_t, sp[-1], tos);
	NEXT();

	ASMLABEL(rot_code);
rot_code: /*: ROT ( rot ) ;*/
	ROT(cell_t, sp[-2], sp[-1], tos);
	NEXT();

	ASMLABEL(over_code);
over_code: /*: OVER ( over ) ;*/
	PUSH(sp) = tos;
	tos = sp[-2];
	NEXT();

	ASMLABEL(nip_code);
nip_code: /*: NIP ( nip ) ;*/
	(void)POP(sp);
	NEXT();

	ASMLABEL(tuck_code);
tuck_code: /*: TUCK ( tuck ) ;*/
	wp = (void **)sp[-1];
	sp[-1] = tos;
	PUSH(sp) = (cell_t)wp;
	NEXT();

	ASMLABEL(unrot_code);
unrot_code: /*: -ROT ( unrot ) ;*/
	UNROT(cell_t, sp[-2], sp[-1], tos);
	NEXT();

	ASMLABEL(qdup_code);
qdup_code: /*: ?DUP ( qdup ) ;*/
	if (tos)
		PUSH(sp) = tos;
	NEXT();
