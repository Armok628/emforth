exit_c: // EXIT
	asm("exit:");
	ip = (void ***)POP(rp);
	NEXT();

execute_c: // EXECUTE
	asm("execute:");
	wp = (void **)tos;
	tos = POP(sp);
	goto **wp;

branch_c: // BRANCH
	asm("branch:");
	ip = (void ***)((cell)ip + *(cell *)ip);
	NEXT();

qbranch_c: // ?BRANCH
	asm("qbranch:");
	wp = (void **)tos;
	tos = POP(sp);
	if (!wp)
		goto branch_c;
	ip++;
	NEXT();
