exit_c:
	asm("exit:");
	ip = (void ***)POP(rp);
	NEXT();
execute_c:
	asm("execute:");
	wp = (void **)tos;
	tos = POP(sp);
	goto **wp;
branch_c:
	asm("branch:");
	ip += *(cell *)ip;
	NEXT();
qbranch_c:
	asm("qbranch:");
	wp = (void **)tos;
	tos = POP(sp);
	if (!wp)
		goto branch_c;
	ip++;
	NEXT();
