#include "fthutil.h"
#include "sysif/sysif.h"

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

	#include "engine.c"
}

void init_cfs(void)
{
	engine(0, 0, 0, 0, 0, 0);
}

int main(int argc, char *argv[])
{
	init_cfs();

	static void **test[] = {
		XT(xio),
		XT(keyq),
		XT(qbranch),
		(void **)(-2*sizeof(cell)),
		XT(bye)
	};
	cell sp[16], rp[16];

	return *engine(test,sp,rp,0,0,0);
}
