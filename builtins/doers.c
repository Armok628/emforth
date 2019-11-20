docol_code: /*: DOCOL ( docol ) ;*/
	ASMLABEL(docol_code);
	PUSH(rp) = (cell_t)ip;
	ip = (void ***)wp + 1;
	goto next;

dolit_code: /*: DOLIT ( dolit ) ;*/
	ASMLABEL(dolit_code);
	PUSH(sp) = tos;
	tos = *(cell_t *)ip++;
	goto next;

doconst_code: /*: DOCONST ( doconst ) ;*/
	ASMLABEL(doconst_code);
	PUSH(sp) = tos;
	tos = (cell_t)wp[1];
	goto next;

dovar_code: /*: DOVAR ( dovar ) ;*/
	ASMLABEL(dovar_code);
	PUSH(sp) = tos;
	tos = (cell_t)wp + 1;
	goto next;

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
	goto next;
// TODO: Would it be worthwile to replace some DOers with DODOES>?
