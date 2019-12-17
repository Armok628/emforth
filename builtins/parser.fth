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
	DUP 0> INVERT IF 2DROP -1 EXIT THEN
	KEY DUP 4 = IF DROP 2DROP -1 EXIT THEN
	BEGIN ( c-addr u char )
		DUP EMIT
		ROT TUCK C! SWAP 1 /STRING
		KEY OVER 0> INVERT OVER DUP 4 = SWAP 13 = OR OR
	UNTIL
	DROP NIP
;

: REFILL ( refill ) ( -- flag )
	SOURCE-ID 0= IF
		TIB /TIB ACCEPT
		TIB >SOURCE !
		DUP /SOURCE !
		0< INVERT
		0 >IN !
	ELSE
		FALSE
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
