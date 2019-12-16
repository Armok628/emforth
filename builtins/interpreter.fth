: >NAME ( to_name ) ( xt -- c-addr u )
	CELL+ DUP @ SWAP CELL+ @
;
: >XT ( to_xt ) ( nt -- xt )
	3 CELLS +
;

: FIND-NAME ( find_name ) ( c-addr u wid -- xt -1 | c-addr u 0 )
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
: DEFINED? ( defined ) ( c-addr u -- xt flag | c-addr u 0 )
	LATEST FIND-NAME
;

: S>D ( s_to_d ) DUP 0< ;
: NUMBER? ( number ) ( c-addr u -- n -1 | c-addr u 0 )
	2DUP 0 S>D 2SWAP >NUMBER 0= IF
		2DROP >R 2DROP R>
		TRUE
	ELSE
		DROP 2DROP
		FALSE
	THEN
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
