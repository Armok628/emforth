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
