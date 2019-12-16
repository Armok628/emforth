lit_code: /*: LIT ( lit ) ;*/
	ASMLABEL(lit_code);
	PUSH(sp) = tos;
	tos = *(cell_t *)ip++;
	NEXT();

dup_code: /*: DUP ( dup ) ;*/
	ASMLABEL(dup_code);
	PUSH(sp) = tos;
	NEXT();

drop_code: /*: DROP ( drop ) ;*/
	ASMLABEL(drop_code);
	tos = POP(sp);
	NEXT();

swap_code: /*: SWAP ( swap ) ;*/
	ASMLABEL(swap_code);
	SWAP(cell_t, sp[-1], tos);
	NEXT();

rot_code: /*: ROT ( rot ) ;*/
	ASMLABEL(rot_code);
	ROT(cell_t, sp[-2], sp[-1], tos);
	NEXT();

over_code: /*: OVER ( over ) ;*/
	ASMLABEL(over_code);
	PUSH(sp) = tos;
	tos = sp[-2];
	NEXT();

nip_code: /*: NIP ( nip ) ;*/
	ASMLABEL(nip_code);
	(void)POP(sp);
	NEXT();

tuck_code: /*: TUCK ( tuck ) ;*/
	ASMLABEL(tuck_code);
	wp = (void **)sp[-1];
	sp[-1] = tos;
	PUSH(sp) = (cell_t)wp;
	NEXT();

unrot_code: /*: -ROT ( unrot ) ;*/
	ASMLABEL(unrot_code);
	UNROT(cell_t, sp[-2], sp[-1], tos);
	NEXT();

qdup_code: /*: ?DUP ( qdup ) ;*/
	ASMLABEL(qdup_code);
	if (tos)
		PUSH(sp) = tos;
	NEXT();
