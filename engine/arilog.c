#define OP2(name,op) \
name##_c: \
	asm(#name":"); \
	tos = POP(sp) op tos; \
	NEXT();
OP2(add,+)
OP2(and,&)
OP2(or,|)
OP2(xor,^)
zlt_c:
	asm("zlt:");
	tos = (tos < 0) ? ~0 : 0;
	NEXT();
