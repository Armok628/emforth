#ifndef FTHDEF_H
#define FTHDEF_H
#include <stdint.h>

#define CELL64 (INTPTR_MAX == 0x7FFFFFFFFFFFFFFFLL)
#define CELL32 (INTPTR_MAX == 0x7FFFFFFFL)
#define CELL16 (INTPTR_MAX == 0x7FFF)

#if CELL64
	#ifdef EMULATE_DOUBLES
		typedef int64_t dcell_t;
		typedef uint64_t udcell_t;
	#else
		typedef __int128_t dcell_t;
		typedef __uint128_t udcell_t;
	#endif
	typedef int64_t cell_t;
	typedef uint64_t ucell_t;
#elif CELL32
	#ifdef EMULATE_DOUBLES
		typedef int32_t dcell_t;
		typedef uint32_t udcell_t;
	#else
		typedef int64_t dcell_t;
		typedef uint64_t udcell_t;
	#endif
	typedef int32_t cell_t;
	typedef uint32_t ucell_t;
#elif CELL16
	#ifdef EMULATE_DOUBLES
		typedef int16_t dcell_t;
		typedef uint16_t udcell_t;
	#else
		typedef int32_t dcell_t;
		typedef uint32_t udcell_t;
	#endif
	typedef int16_t cell_t;
	typedef uint16_t ucell_t;
#else
	#error
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

#define COUNT(a) (sizeof(a) / sizeof(a[0]))
#define PUSH(x) *(x++)
#define POP(x) *(--x)
#endif
