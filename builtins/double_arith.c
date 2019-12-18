um_divmod_code: /*: UM/MOD ( um_divmod ) ;*/
	ASMLABEL(um_divmod_code);
#if defined(__x86_64__)
	sp--;
	asm ("divq %4"
		:"=d"(sp[-1]), "=a"(tos)
		:"d"(sp[0]), "a"(sp[-1]), "r"(tos));
#else
{
	udcell_t d = *(udcell_t *)&sp[-2];
	sp -= 2;
	PUSH(sp) = d % tos;
	tos = d / tos;
}
#endif
	NEXT();

m_add_code: /*: M+ ( m_add ) ;*/
	ASMLABEL(m_add_code);
	*(dcell_t *)&sp[-2] += (ucell_t)tos;
	tos = POP(sp);
	NEXT();

m_mul_code: /*: M* ( m_mul ) ;*/
	ASMLABEL(m_mul_code);
{
	dcell_t d = (dcell_t)tos * sp[-1];
	sp[-1] = (cell_t)d;
	tos = d >> (8 * sizeof(cell_t));
}
	NEXT();
