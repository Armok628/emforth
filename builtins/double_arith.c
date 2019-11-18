#ifdef EMULATE_DOUBLES
const ucell_t lohalf = (1UL << 4 * sizeof(cell_t)) - 1;
#define LOAD_DOUBLE(p) ((p)[0] << 4 * sizeof(cell_t) | (p)[1])
#define STORE_DOUBLE(p,v) \
	do { \
		udcell_t a = (v); \
		*(ucell_t *)&(p)[0] = a >> 4 * sizeof(cell_t); \
		*(ucell_t *)&(p)[1] = a & lohalf; \
	} while (0)
#else
#define LOAD_DOUBLE(p) (*(dcell_t *)(p))
#define STORE_DOUBLE(p,v) \
	do { \
		*(udcell_t *)(p) = (udcell_t)(v); \
	} while (0)
#endif

um_divmod_code: /*: UM/MOD ( um_divmod ) ;*/
	ASMLABEL(um_divmod_code);
#if defined(__x86_64__) && !defined(EMULATE_DOUBLES)
	sp--;
	asm ("divq %4"
		:"=d"(sp[-1]), "=a"(tos)
		:"d"(sp[0]), "a"(sp[-1]), "r"(tos));
#else
	udcell_t d = LOAD_DOUBLE(&sp[-2]);
	sp -= 2;
	PUSH(sp) = d % tos;
	tos = d / tos;
#endif
	goto next;

m_add_code: /*: M+ ( m_add ) ;*/
	ASMLABEL(m_add_code);
	STORE_DOUBLE(&sp[-2], LOAD_DOUBLE(&sp[-2]) + (ucell_t)tos);
	tos = POP(sp);
	goto next;
