dup_code: /*: DUP ( dup ) ;*/
	ASMLABEL(dup_code);
	PUSH(sp) = tos;
	goto next;

drop_code: /*: DROP ( drop ) ;*/
	ASMLABEL(drop_code);
	tos = POP(sp);
	goto next;

swap_code: /*: SWAP ( swap ) ;*/
	ASMLABEL(swap_code);
	wp = (void **)sp[-1];
	sp[-1] = tos;
	tos = (cell_t)wp;
	goto next;

rot_code: /*: ROT ( rot ) ;*/
	ASMLABEL(rot_code);
	wp = (void **)sp[-2];
	sp[-2] = sp[-1];
	sp[-1] = tos;
	tos = (cell_t)wp;
	goto next;

over_code: /*: OVER ( over ) ;*/
	ASMLABEL(over_code);
	PUSH(sp) = tos;
	tos = sp[-2];
	goto next;

nip_code: /*: NIP ( nip ) ;*/
	ASMLABEL(nip_code);
	(void)POP(sp);
	goto next;

tuck_code: /*: TUCK ( tuck ) ;*/
	ASMLABEL(tuck_code);
	wp = (void **)sp[-1];
	sp[-1] = tos;
	PUSH(sp) = (cell_t)wp;
	goto next;

unrot_code: /*: -ROT ( unrot ) ;*/
	ASMLABEL(unrot_code);
	wp = (void **)sp[-1];
	sp[-1] = sp[-2];
	sp[-2] = tos;
	tos = (cell_t)wp;
	goto next;

qdup_code: /*: ?DUP ( qdup ) ;*/
	ASMLABEL(qdup_code);
	if (tos)
		PUSH(sp) = tos;
	goto next;
