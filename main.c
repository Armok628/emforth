#include "fthdef.h"

#ifdef NO_ASMLABELS
#define ASMLABEL(x)
#else
#define ASMLABEL(x) asm ("."#x":")
#endif

#include "dict.c"
void engine(FTH_REGS)
{
	#include "cfs.c"
	if (!ip) {
		struct fthdef *d = &forth_wordlist_def;
		for (size_t i = 0; i < COUNT(cfs); i++) {
			d->cf = cfs[i];
			d = d->prev;
		}
		return;
	}

	cell_t *sp0 = sp;
	(void)sp0;

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
	init_cfs();
	run_thread(quit_def.data);
	return 0;
}
