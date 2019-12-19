VARIABLE STATE ( state )
0 STATE !

: >NAME ( to_name ) ( xt -- c-addr u )
	CELL+ DUP @ SWAP CELL+ @
;
: >XT ( to_xt ) ( nt -- xt )
	3 CELLS +
;

: SEARCH-WORDLIST ( search_wordlist ) ( c-addr u wid -- 0 | xt +/-1 )
	BEGIN
		@ ?DUP
	WHILE
		>R
		2DUP R@
		>NAME COMPARE 0= IF
			2DROP R> TRUE
			( OVER IMMEDIATE? IF NEGATE THEN )
			EXIT
		THEN
		R>
	REPEAT
	FALSE
;
: DEFINED? ( defined ) ( c-addr u -- xt +/-1 | c-addr u 0 )
	\ TODO Multiple wordlist handling
	2DUP LATEST SEARCH-WORDLIST DUP IF
		2>R 2DROP 2R>
	THEN
;

: S>D ( s_to_d ) DUP 0< ;
: NUMBER? ( number ) ( c-addr u -- n -1 | c-addr u 0 )
	DUP 0> INVERT IF FALSE EXIT THEN
	BASE @ >R
	OVER C@ >R
	     R@ [CHAR] $ = IF
		16 BASE ! 1 /STRING
	ELSE R@ [CHAR] # = IF
		10 BASE ! 1 /STRING
	ELSE R@ [CHAR] % = IF
		2 BASE ! 1 /STRING
	THEN THEN THEN R> DROP

	OVER C@ [CHAR] - = IF 1 /STRING -1 ELSE 0 THEN >R

	0 S>D 2OVER >NUMBER DUP 0= IF
		2DROP DROP >R 2DROP R>
		R> IF NEGATE THEN
		TRUE
	ELSE
		2DROP 2DROP
		R> DROP
		FALSE
	THEN
	R> BASE !
;

\ DEFER COMPILE,
\ ' , IS COMPILE,
: COMPILE, ( compile_comma ) , ;

: LITERAL ( literal ) ( x -- )
	LIT LIT , COMPILE,
; IMMEDIATE

: INTERPRET-NAME ( interpret_name ) ( i*x c-addr u -- j*x )
	STATE @ IF
		DEFINED? ?DUP IF
			0> IF
				EXECUTE
			ELSE
				COMPILE,
			THEN
		ELSE NUMBER? IF
			POSTPONE LITERAL
		ELSE
			( -14 THROW )
			[CHAR] ? EMIT
		THEN THEN
	ELSE
		DEFINED? IF
			EXECUTE
		ELSE NUMBER? INVERT IF
			( -14 THROW )
			[CHAR] ? EMIT
		THEN THEN
	THEN
;

: INTERPRET ( interpret ) ( i*x -- j*x )
	BEGIN
		PARSE-NAME
		DUP
	WHILE
		INTERPRET-NAME
	REPEAT
;

: QUIT ( quit ) ( i*x -- j*x )
	BEGIN
		REFILL INVERT
	WHILE
		( ['] ) INTERPRET ( CATCH )
	REPEAT
	BYE
;
