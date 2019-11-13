#include <stddef.h>
#include "fthdef.h"

#ifdef USE_ASMLABELS
#define ASMLABEL(x) __asm__(#x ":")
#else
#define ASMLABEL(x)
#endif

#define countof(a) (sizeof(a) / sizeof(a[0]))

#include "dict.c"

void engine(FTH_REGS)
{
	#include "cfas.c"
	if (!ip) {
		struct primitive *d = latest;
		for (size_t i = 0; i < countof(cfas); i++) {
			d->cfa = cfas[i];
			d = d->prev;
		}
		return;
	}

next:
	ASMLABEL(next);
	goto **(wp = *(ip++));

bye_code: /*: BYE ( bye ) ;*/
	ASMLABEL(bye_code);
	PUSH(sp) = tos;
	return;

docol_code: /*: DOCOL ( docol ) ;*/
	ASMLABEL(docol_code);
	PUSH(rp) = (cell_t)ip;
	ip = (void ***)wp + 1;
	goto next;
exit_code: /*: EXIT ( exit ) ;*/
	ASMLABEL(exit_code);
	ip = (void ***)POP(rp);
	goto next;
dolit_code: /*: DOLIT ( dolit ) ;*/
	ASMLABEL(dolit_code);
	PUSH(sp) = tos;
	tos = *(cell_t *)ip++;
	goto next;
execute_code: /*: EXECUTE ( execute ) ;*/
	ASMLABEL(execute_code);
	wp = (void **)tos;
	tos = POP(sp);
	goto **wp;

branch_code: /*: BRANCH ( branch ) ;*/
	ASMLABEL(branch_code);
	ip = (void ***)((char *)ip + *(cell_t *)ip);
	tos = POP(sp);
	goto next;
zbranch_code: /*: 0BRANCH ( zbranch ) ;*/
	ASMLABEL(zbranch_code);
	if (tos)
		goto drop_code;
	else
		goto branch_code;
goto_code: /*: GO-TO ( goto ) ;*/
	ASMLABEL(goto_code);
	ip = *(void ****)ip;
	goto next;

dup_code: /*: DUP ( dup ) ;*/
	ASMLABEL(dup_code);
	PUSH(sp) = tos;
	goto next;
drop_code: /*: DROP ( drop ) ;*/
	ASMLABEL(drop_code);
	tos = POP(sp);
	goto next;
swap_code: /*: SWAP ( swap ) ;*/
	ASMLABEL(swap_code);
	wp = (void **)sp[-1];
	sp[-1] = tos;
	tos = (cell_t)wp;
	goto next;
rot_code: /*: ROT ( rot ) ;*/
	ASMLABEL(rot_code);
	wp = (void **)sp[-2];
	sp[-2] = sp[-1];
	sp[-1] = tos;
	tos = (cell_t)wp;
	goto next;

over_code: /*: OVER ( over ) ;*/
	ASMLABEL(over_code);
	PUSH(sp) = tos;
	tos = sp[-2];
	goto next;
nip_code: /*: NIP ( nip ) ;*/
	ASMLABEL(nip_code);
	(void)POP(sp);
	goto next;
tuck_code: /*: TUCK ( tuck ) ;*/
	ASMLABEL(tuck_code);
	wp = (void **)sp[-1];
	sp[-1] = tos;
	PUSH(sp) = (cell_t)wp;
	goto next;
unrot_code: /*: -ROT ( unrot ) ;*/
	ASMLABEL(unrot_code);
	wp = (void **)sp[-1];
	sp[-1] = sp[-2];
	sp[-2] = tos;
	tos = (cell_t)wp;
	goto next;

store_code: /*: ! ( store ) ;*/
	ASMLABEL(store_code);
	*(cell_t *)tos = POP(sp);
	tos = POP(sp);
	goto next;
fetch_code: /*: @ ( fetch ) ;*/
	ASMLABEL(fetch_code);
	tos = *(cell_t *)tos;
	goto next;
c_store_code: /*: C! ( c_store ) ;*/
	ASMLABEL(c_store_code);
	*(char *)tos = (char)POP(sp);
	tos = POP(sp);
	goto next;
c_fetch_code: /*: C@ ( c_fetch ) ;*/
	ASMLABEL(c_fetch_code);
	tos = *(char *)tos;
	goto next;
addstore_code: /*: +! ( addstore ) ;*/
	ASMLABEL(addstore_code);
	*(cell_t *)tos += POP(sp);
	tos = POP(sp);
	goto next;

here_code: /*: HERE ( here ) ;*/
	ASMLABEL(here_code);
	PUSH(sp) = tos;
	tos = (cell_t)dp;
	goto next;
allot_code: /*: ALLOT ( allot ) ;*/
	ASMLABEL(allot_code);
	dp = (cell_t *)((char *)dp + tos);
	tos = POP(sp);
	goto next;
comma_code: /*: , ( comma ) ;*/
	ASMLABEL(comma_code);
	*dp = tos;
	dp++;
	tos = POP(sp);
	goto next;
c_comma_code: /*: C, ( c_comma ) ;*/
	ASMLABEL(c_comma_code);
	*(char *)dp = (char)tos;
	dp = (cell_t *)((char *)dp+1);
	tos = POP(sp);
	goto next;

to_r_code: /*: >R ( to_r ) ;*/
	ASMLABEL(to_r_code);
	PUSH(rp) = tos;
	tos = POP(sp);
	goto next;
r_from_code: /*: R> ( r_from ) ;*/
	ASMLABEL(r_from_code);
	PUSH(sp) = tos;
	tos = POP(rp);
	goto next;
r_fetch_code: /*: R@ ( r_fetch ) ;*/
	ASMLABEL(r_fetch_code);
	PUSH(sp) = tos;
	tos = rp[-1];
	goto next;

#define OP2(n,a,b) \
n##_code: ASMLABEL(n##_code); \
	tos = b(POP(sp) a tos); \
	goto next;
OP2(add,+,) /*: + ( add ) ;*/
OP2(sub,-,) /*: - ( sub ) ;*/
OP2(mul,*,) /*: * ( mul ) ;*/
OP2(div,/,) /*: / ( div ) ;*/
OP2(lsh,<<,) /*: LSHIFT ( lsh ) ;*/
OP2(rsh,>>,) /*: RSHIFT ( rsh ) ;*/

OP2(gt,>,-) /*: > ( gt ) ;*/
OP2(gte,>=,-) /*: >= ( gte ) ;*/
OP2(lt,<,-) /*: < ( lt ) ;*/
OP2(lte,<=,-) /*: <= ( lte ) ;*/
OP2(eq,==,-) /*: = ( eq ) ;*/
OP2(neq,!=,-) /*: <> ( neq ) ;*/

#define OP1(n,a,b) \
n##_code: ASMLABEL(n##_code); \
	tos = a(tos b); \
	goto next;
OP1(inc,,+1) /*: 1+ ( inc ) ;*/
OP1(dec,,-1) /*: 1- ( dec ) ;*/
OP1(neg,-,) /*: NEGATE ( neg ) ;*/
OP1(not,~,) /*: INVERT ( not ) ;*/
OP1(lsh1,,<<1) /*: 2* ( lsh1 ) ;*/
OP1(rsh1,,>>1) /*: 2/ ( rsh1 ) ;*/

OP1(gtz,-,>0) /*: 0> ( gtz ) ;*/
OP1(gtez,-,>=0) /*: 0>= ( gtez ) ;*/
OP1(ltz,-,<0) /*: 0< ( ltz ) ;*/
OP1(ltez,-,<=0) /*: 0<= ( ltez ) ;*/
OP1(eqz,-,==0) /*: 0= ( eqz ) ;*/
OP1(neqz,-,!=0) /*: 0<> ( neqz ) ;*/

}

void init_cfas(void)
{
	engine(0, 0, 0, 0, 0, 0);
}

void thread(void ***ip0)
{
	cell_t sp0[64], rp0[32], dp0[1024];

	engine(ip0, sp0, rp0, dp0, NULL, 0);
}

int main()
{
	static void **test[] = {
		&dolit_def.cfa, (void **)2,
		&dup_def.cfa,
		&dolit_def.cfa, &add_def.cfa, &execute_def.cfa,
		&bye_def.cfa
	};

	init_cfas();
	thread(test);
	return 0;
}
