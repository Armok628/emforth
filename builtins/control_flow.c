	ASMLABEL(exit_code);
exit_code: /*: EXIT ( exit ) ;*/
	ip = (void ***)POP(rp);
	NEXT();

	ASMLABEL(bye_code);
bye_code: /*: BYE ( bye ) ;*/
	PUSH(sp) = tos;
	return;

	ASMLABEL(branch_code);
branch_code: /*: BRANCH ( branch ) ;*/
	ip = (void ***)((char *)ip + *(cell_t *)ip);
	NEXT();

	ASMLABEL(zbranch_code);
zbranch_code: /*: 0BRANCH ( zbranch ) ;*/
	if (!tos) {
		tos = POP(sp);
		goto branch_code;
	} else {
		tos = POP(sp);
		ip++;
		goto drop_code;
	}

	ASMLABEL(goto_code);
goto_code: /*: GO-TO ( goto ) ;*/
	ip = *(void ****)ip;
	NEXT();

	ASMLABEL(execute_code);
execute_code: /*: EXECUTE ( execute ) ;*/
	wp = (void **)tos;
	tos = POP(sp);
	goto **wp;
