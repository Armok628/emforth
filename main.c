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
	#define XT(x) &x##_def.cf
	#define IMM(x) (void **)(x)
	#define CELLS * sizeof(cell_t)
	#define LIT(x) XT(lit),IMM(x)
	static void **test[] = {
		XT(refill),
		XT(drop),
		XT(cr),

		XT(parse_name),
		XT(number),
		XT(not),

		XT(zbranch),
		IMM(5 CELLS),
		XT(two_drop),
		LIT('?'),
		XT(dup),
		XT(emit),

		XT(emit),
		XT(bye),
	};

	init_cfs();
	run_thread(test);
	return 0;
}
