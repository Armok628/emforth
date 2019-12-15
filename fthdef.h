#ifndef FTHDEF_H
#define FTHDEF_H
#include "cell.h"

#define FTH_REGS \
	register void **ip[], \
	register cell_t *sp, \
	register cell_t *rp, \
	cell_t *dp, \
	register void **wp, \
	register cell_t tos

struct fthdef {
	struct fthdef *prev;
	char *name;
	cell_t namelen;
	void *cf;
	void **data[];
};

const ucell_t msb = ~(~0UL>>1);

#define COUNT(a) (sizeof(a) / sizeof(a[0]))
#define PUSH(x) *(x++)
#define POP(x) *(--x)
#endif
