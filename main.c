#include <unistd.h>
#include <errno.h>
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
	//ASMLABEL(next);
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
dolit_code: /*: DOLIT ( dolit ) ;*/
	ASMLABEL(dolit_code);
	PUSH(sp) = tos;
	tos = *(cell_t *)ip++;
	goto next;
exit_code: /*: EXIT ( exit ) ;*/
	ASMLABEL(exit_code);
	ip = (void ***)POP(rp);
	goto next;

doconst_code: /*: DOCONST ( doconst ) ;*/
	ASMLABEL(doconst_code);
	PUSH(sp) = tos;
	tos = *(cell_t *)(wp+1);
	goto next;
dovar_code: /*: DOVAR ( dovar ) ;*/
	ASMLABEL(dovar_code);
	PUSH(sp) = tos;
	tos = (cell_t)(wp+1);
	goto next;
dodefer_code: /*: DODEFER ( dodefer ) ;*/
	ASMLABEL(dovar_code);
	wp = *(void ***)(wp+1);
	goto **wp;

execute_code: /*: EXECUTE ( execute ) ;*/
	ASMLABEL(execute_code);
	wp = (void **)tos;
	tos = POP(sp);
	goto **wp;

#include "prims.c"

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
