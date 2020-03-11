#include "fthutil.h"
#include "sysif/sysif.h"

#include "vocab.h"
VOCAB(DEF)

cell *engine(FTHREGS)
{
	static void **cfs[] = {VOCAB(CF)};

	if (ip == NULL) {
		// If IP is NULL, initialize primitive code fields
		struct fthdef *d = LAST_VOC;
		for (int i = COUNT(cfs)-1; i >= 0; i--) {
			d->cf = cfs[i];
			d = d->prev;
		}
		return NULL;
	}
	// Initialize I/O and jump to IP
	manage_io(IO_INIT);
	NEXT();

	#include "engine.c"
}

#define init_cfs() engine(0, 0, 0, 0, 0, 0)

int main(int argc, char *argv[])
{
	init_cfs();

	static void **test[] = {
		XT(keyq),
		XT(qbranch),
		(void **)(-2*sizeof(cell)),
		XT(bye)
	};
	cell sp[16], rp[16];

	return *engine(test,sp,rp,0,0,0);
}
