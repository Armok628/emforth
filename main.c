#include "fthutil.h"

#include "vocab.h"
VOCAB(DEF)

cell *engine(FTHREGS)
{
	static void **cfs[] = {VOCAB(CF)};

	if (ip == NULL) {
		struct fthdef *d = LAST_VOC;
		for (int i = COUNT(cfs)-1; i >= 0; i--) {
			d->cf = cfs[i];
			d = d->prev;
		}
		return NULL;
	}
	NEXT();

	// System interface
bye_c:
	asm("bye:");
	*sp = tos;
	return sp;

	// Inner interpreters
docol_c:
	asm("docol:");
	PUSH(rp) = (cell)ip;
	ip = (void ***)&wp[1];
	NEXT();

	// Control flow
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

	// Data stack
spfetch_c:
	asm("spfetch:");
	PUSH(sp) = tos;
	tos = (cell)sp;
	NEXT();
spstore_c:
	asm("spstore:");
	sp = (cell *)tos;
	tos = POP(sp);
	NEXT();
dolit_c:
	asm("dolit:");
	PUSH(sp) = tos;
	tos = *(cell *)ip++;
	NEXT();
dup_c:
	asm("dup:");
	PUSH(sp) = tos;
	NEXT();
drop_c:
	asm("drop:");
	tos = POP(sp);
	NEXT();
swap_c:
	asm("swap:");
	SWAP(cell, tos,sp[-1]);
	NEXT();
over_c:
	asm("over:");
	PUSH(sp) = tos;
	tos = sp[-2];
	NEXT();

	// Return stack
rpfetch_c:
	asm("rpfetch:");
	PUSH(sp) = tos;
	tos = (cell)rp;
	NEXT();
rpstore_c:
	asm("rpstore:");
	rp = (cell *)tos;
	tos = POP(sp);
	NEXT();
to_r_c:
	asm("to_r:");
	PUSH(rp) = tos;
	tos = POP(sp);
	NEXT();
r_fetch_c:
	asm("r_fetch:");
	PUSH(sp) = tos;
	tos = rp[-1];
	NEXT();
r_from_c:
	asm("r_from:");
	PUSH(sp) = tos;
	tos = POP(rp);
	NEXT();

	// Memory access
store_c:
	asm("store:");
	*(cell *)tos = POP(sp);
	tos = POP(sp);
	NEXT();
fetch_c:
	asm("fetch:");
	tos = *(cell *)tos;
	NEXT();
cstore_c:
	asm("cstore:");
	*(char *)tos = POP(sp);
	tos = POP(sp);
	NEXT();
cfetch_c:
	asm("cfetch:");
	tos = *(char *)tos;
	NEXT();

	// Arithmetic/Logic
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
}

void init_cfs(void)
{
	engine(0, 0, 0, 0, 0, 0);
}

int main(int argc, char *argv[])
{
	init_cfs();

	static void **test[] = {XT(cell),XT(dup),XT(add),XT(bye)};
	cell sp[16], rp[16];


	return *engine(test,sp,rp,0,0,0);
}
