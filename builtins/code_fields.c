	ASMLABEL(docol_code);
docol_code: /*: DOCOL ( docol ) ;*/
	PUSH(rp) = (cell_t)ip;
	ip = (void ***)wp + 1;
	NEXT();

	ASMLABEL(doconst_code);
docon_code: /*: DOCON ( docon ) ;*/
	PUSH(sp) = tos;
	tos = (cell_t)wp[1];
	NEXT();

	ASMLABEL(dovar_code);
dovar_code: /*: DOVAR ( dovar ) ;*/
	PUSH(sp) = tos;
	tos = (cell_t)wp + 1;
	NEXT();

	ASMLABEL(dodefer_code);
dodefer_code: /*: DODEFER ( dodefer ) ;*/
	wp = (void **)wp[1];
	goto **wp;

	ASMLABEL(dodoes_code);
dodoes_code: /*: DODOES ( dodoes ) ;*/
	PUSH(rp) = (cell_t)ip;
	ip = (void ***)wp[1];
	PUSH(sp) = tos;
	tos = (cell_t)wp + 2;
	NEXT();
