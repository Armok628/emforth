#ifndef FTHDEF_H
#define FTHDEF_H
#include <stdint.h>
#if INTPTR_MAX > 0xFFFFFFFF
typedef unsigned __int128 udcell_t;
typedef __int128 dcell_t;
typedef int64_t cell_t;
typedef uint64_t ucell_t;
#elif INTPTR_MAX > 0xFFFF
typedef uint64_t udcell_t;
typedef int64_t dcell_t;
typedef int32_t cell_t;
typedef uint32_t ucell_t;
#else
typedef uint32_t udcell_t;
typedef int32_t dcell_t;
typedef int16_t cell_t;
typedef uint16_t ucell_t;
#endif

#define FTH_REGS \
	register void **ip[], \
	register cell_t *sp, \
	register cell_t *rp, \
	cell_t *dp, \
	register void **wp, \
	register cell_t tos

struct primitive {
	struct primitive *prev;
	char *name;
	cell_t namelen;
	void *cfa;
	void **data[];
};

const ucell_t msb = ~(~0UL>>1);

#define PUSH(x) *(x++)
#define POP(x) *(--x)
#endif
