exit_code: /*: EXIT ( exit ) ;*/
	ASMLABEL(exit_code);
	ip = (void ***)POP(rp);
	NEXT();

bye_code: /*: BYE ( bye ) ;*/
	ASMLABEL(bye_code);
	PUSH(sp) = tos;
	return;

branch_code: /*: BRANCH ( branch ) ;*/
	ASMLABEL(branch_code);
	ip = (void ***)((char *)ip + *(cell_t *)ip);
	tos = POP(sp);
	NEXT();

zbranch_code: /*: 0BRANCH ( zbranch ) ;*/
	ASMLABEL(zbranch_code);
	if (tos)
		goto drop_code;
	else
		goto branch_code;

goto_code: /*: GO-TO ( goto ) ;*/
	ASMLABEL(goto_code);
	ip = *(void ****)ip;
	NEXT();

execute_code: /*: EXECUTE ( execute ) ;*/
	ASMLABEL(execute_code);
	wp = (void **)tos;
	tos = POP(sp);
	goto **wp;
