C\ char tib[256];
C{ &tib } CONSTANT TIB ( tib )
C{ COUNT(tib) } CONSTANT /TIB ( per_tib )

VARIABLE >SOURCE ( to_source )
C{ &tib } >SOURCE !
VARIABLE /SOURCE ( per_source )
0 /SOURCE !

: SOURCE ( source ) >SOURCE @ /SOURCE @ ;
: SOURCE-ID ( source_id ) >SOURCE @ TIB <> ;

VARIABLE >IN ( to_in )
0 >IN !

: ACCEPT ( accept ) ( c-addr u -- u )
	DUP 0<= IF 2DROP -1 EXIT THEN
	KEY DUP 4 = IF DROP 2DROP -1 EXIT THEN
	>R TUCK R>
	BEGIN ( u0 c-addr u char )
		DUP 127 = IF
			DROP
			>R OVER R> TUCK > IF
				27 EMIT [CHAR] [ EMIT [CHAR] D EMIT
				BL EMIT
				27 EMIT [CHAR] [ EMIT [CHAR] D EMIT
				-1 /STRING
			THEN
		ELSE
			DUP 9 = IF BL ELSE DUP THEN EMIT
			ROT TUCK C! SWAP
			1 /STRING
		THEN
		KEY OVER 0<= OVER DUP 4 = SWAP 13 = OR OR
	UNTIL
	DROP BL EMIT NIP -
;

: REFILL ( refill ) ( -- flag )
	SOURCE-ID 0= IF
		TIB /TIB ACCEPT
		TIB >SOURCE !
		DUP /SOURCE !
		0>=
		0 >IN !
	ELSE
		FALSE
	THEN
;

: PARSE-NAME ( parse_name ) ( -- c-addr u )
	SOURCE OVER >R
	>IN @ /STRING
	BEGIN OVER C@ BL <= OVER 0> AND WHILE 1 /STRING REPEAT
	2DUP
	BEGIN OVER C@ BL > OVER 0> AND WHILE 1 /STRING REPEAT
	NIP -
	2DUP + R> - >IN !
;
