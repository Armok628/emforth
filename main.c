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

	static void **ip[] = {
		XT(word),
		XT(cr),
		XT(type),
		XT(cr),
		XT(bye)
	};
	cell sp[64], rp[64], dp[1024];

	return *engine(ip,sp,rp,dp,0,0);
}
