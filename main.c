#include "fthdef.h"

#ifdef USE_ASMLABELS
#define ASMLABEL(x) asm ("."#x":")
#else
#define ASMLABEL(x)
#endif

#include "dict.c"
void engine(FTH_REGS)
{
	#include "cfas.c"
	if (!ip) {
		struct primitive *d = &latest_def;
		for (size_t i = 0; i < COUNT(cfas); i++) {
			d->cfa = cfas[i];
			d = d->prev;
		}
		return;
	}

	ASMLABEL(next);
next:	goto **(wp = *(ip++));

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
		&key_def.cfa,
		&emit_def.cfa,
		&bye_def.cfa
	};

	init_cfas();
	thread(test);
	return 0;
}
