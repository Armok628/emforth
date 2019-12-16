#include "fthdef.h"

#ifdef USE_ASMLABELS
#define ASMLABEL(x) asm ("."#x":")
#else
#define ASMLABEL(x)
#endif

#include "dict.c"
void engine(FTH_REGS)
{
	#include "cfs.c"
	if (!ip) {
		struct fthdef *d = &latest_def;
		for (size_t i = 0; i < COUNT(cfs); i++) {
			d->cf = cfs[i];
			d = d->prev;
		}
		return;
	}

	NEXT();

	#include "prims.c"
}

void init_cfs(void)
{
	engine(0, 0, 0, 0, 0, 0);
}

void run_thread(void ***ip0)
{
	cell_t sp0[64], rp0[32], dp0[1024];

	engine(ip0, sp0, rp0, dp0, NULL, 0);
}

int main()
{
	#define XT(x) &x##_def.cf
	#define LIT(x) XT(lit),(void **)(x)
	static void **test[] = {
		XT(tib),
		XT(dup),
		XT(per_tib),
		XT(accept),
		XT(cr),
		XT(type),
		XT(bye),
	};

	init_cfs();
	run_thread(test);
	return 0;
}
