docol_code: /*: DOCOL ( docol ) ;*/
	ASMLABEL(docol_code);
	PUSH(rp) = (cell_t)ip;
	ip = (void ***)wp + 1;
	next();

docon_code: /*: DOCON ( docon ) ;*/
	ASMLABEL(doconst_code);
	PUSH(sp) = tos;
	tos = (cell_t)wp[1];
	next();

dovar_code: /*: DOVAR ( dovar ) ;*/
	ASMLABEL(dovar_code);
	PUSH(sp) = tos;
	tos = (cell_t)wp + 1;
	next();

dodefer_code: /*: DODEFER ( dodefer ) ;*/
	ASMLABEL(dodefer_code);
	wp = (void **)wp[1];
	goto **wp;

dodoes_code: /*: DODOES> ( dodoes ) ;*/
	ASMLABEL(dodoes_code);
	PUSH(rp) = (cell_t)ip;
	ip = (void ***)wp[1];
	PUSH(sp) = tos;
	tos = (cell_t)wp + 2;
	next();
