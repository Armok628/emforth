#define OP2(name,op) \
	asm(#name":"); \
	tos = POP(sp) op tos; \
	NEXT();

add_c: // +
	OP2(add,+)
sub_c: // -
	OP2(sub,-)
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
