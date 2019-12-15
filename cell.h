#ifndef CELL_H
#define CELL_H

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

#endif
