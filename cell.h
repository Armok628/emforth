#ifndef CELL_H
#define CELL_H

#include <stdint.h>

#define CELL64 (INTPTR_MAX == 0x7FFFFFFFFFFFFFFFLL)
#define CELL32 (INTPTR_MAX == 0x7FFFFFFFL)
#define CELL16 (INTPTR_MAX == 0x7FFF)

#if CELL64
typedef __int128_t dcell_t;
typedef __uint128_t udcell_t;
typedef int64_t cell_t;
typedef uint64_t ucell_t;
#elif CELL32
typedef int64_t dcell_t;
typedef uint64_t udcell_t;
typedef int32_t cell_t;
typedef uint32_t ucell_t;
#elif CELL16
typedef int32_t dcell_t;
typedef uint32_t udcell_t;
typedef int16_t cell_t;
typedef uint16_t ucell_t;
#else
	#error
#endif

#endif
