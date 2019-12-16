C\ char tib[256];
C{ &tib } CONSTANT TIB ( tib )
C{ COUNT(tib) } CONSTANT /TIB ( per_tib )

VARIABLE /SOURCE ( per_source )
0 /SOURCE !
VARIABLE >SOURCE ( to_source )
C{ &tib } >SOURCE !
VARIABLE >IN ( to_in )
0 >IN !

: SOURCE ( source ) >SOURCE @ /SOURCE @ ;
: SOURCE-ID ( source_id ) >SOURCE @ TIB <> ;

: ACCEPT ( accept ) ( c-addr u -- u )
	0 2>R
	BEGIN
		2R@ > INVERT IF
			TRUE
		ELSE KEY DUP 4 <> IF
			DUP EMIT
			TUCK OVER C! 1+
			SWAP 10 =
			R> 1+ >R
		ELSE
			DROP
			TRUE
		THEN THEN
	UNTIL
	DROP 2R> NIP
;

: REFILL ( refill ) ( -- u )
	SOURCE-ID IF
		FALSE
	ELSE
		TIB /TIB ACCEPT
		TIB >SOURCE !
		DUP /SOURCE !
		0 >IN !
		0< INVERT
	THEN
;

: PARSE-NAME ( parse_name ) ( -- c-addr u )
	SOURCE >IN @ /STRING
	BEGIN OVER C@ BL > INVERT OVER 0> AND WHILE 1 /STRING REPEAT
	2DUP
	BEGIN OVER C@ BL > OVER 0> AND WHILE 1 /STRING REPEAT
	DUP >IN !
	NIP -
;
