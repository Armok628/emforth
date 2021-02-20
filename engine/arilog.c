#define OP2(name,op) \
	asm(#name":"); \
	tos = POP(sp) op tos; \
	NEXT();

// Zero extend cell to double cell
#define ZXC(x) ((dcell)(ucell)x)
#define CELL_BITS (8*sizeof(cell))

m_add_c: // M+
	asm("m_add:");
{
	register dcell d = ZXC(tos) + ZXC(sp[-1]);
	sp[-1] = (ucell)d;
	tos = d >> CELL_BITS;
}
	NEXT();

and_c: // AND
	OP2(and,&)

or_c: // OR
	OP2(or,|)

xor_c: // XOR
	OP2(xor,^)

zlt_c: // 0<
	asm("zlt:");
	tos = (tos < 0) ? ~0 : 0;
	NEXT();

um_divmod_c: // UM/MOD
	asm("um_divmod:");
{
	register dcell d = ZXC(tos) << CELL_BITS | ZXC(sp[-1]);
	sp[-1] = d % ZXC(tos);
	tos = d / ZXC(tos);
}
	NEXT();

