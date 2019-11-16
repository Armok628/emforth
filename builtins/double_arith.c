#ifdef EMULATE_DOUBLES
#define POP_DOUBLE(r) (r -= 2, r[0] << 4 * sizeof(cell_t) | r[1])
#else
#define POP_DOUBLE(r) (r -= 2, *(udcell_t *)&r[-2])
#endif

um_divmod_code: /*: UM/MOD ( um_divmod ) ;*/
	ASMLABEL(um_divmod_code);
#if defined(__x86_64__) && !defined(EMULATE_DOUBLES)
	sp--;
	asm ("divq %4"
		:"=d"(sp[-1]), "=a"(tos)
		:"d"(sp[0]), "a"(sp[-1]), "r"(tos));
#else
	udcell_t d = POP_DOUBLE(sp);
	PUSH(sp) = d % tos;
	tos = d / tos;
#endif
	goto next;
