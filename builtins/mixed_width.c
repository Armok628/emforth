/*: UM/MOD ( umdivmod ) ;*/
#if !defined(EMULATE_DOUBLES)
umdivmod_code:
	ASMLABEL(umdivmod_code);
#if defined(__x86_64__)
	__asm__("divq %4"
		:"=d"(sp[-2]), "=a"(tos)
		:"d"(sp[-1]), "a"(sp[-2]), "r"(tos)
		:);
	sp--;
#else
{
	udcell_t a = *(udcell_t *)&sp[-2];
	sp -= 2;
	PUSH(sp) = a % tos;
	tos = a / tos;
}
#endif
	goto next;
#else // defined(EMULATE_DOUBLES)
umdivmod_code:
	ASMLABEL(umdivmod_code);
{
	ucell_t a = sp[-2] << 4 * sizeof(cell_t) | sp[-1];
	sp -= 2;
	ucell_t b = tos;
	PUSH(sp) = a % b;
	tos = a / b;
}
	goto next;
#endif
