C\ char tib[256];
C{ &tib } CONSTANT TIB ( tib )
C{ COUNT(tib) } CONSTANT /TIB ( pertib )

VARIABLE /SOURCE ( per_source )
0 /SOURCE !
VARIABLE >SOURCE ( to_source )
C{ &tib } >SOURCE !
VARIABLE >IN ( to_in )
0 >IN !

: SOURCE ( source ) >SOURCE @ /SOURCE @ ;

: REFILL ( refill ) ( -- n )
	0 >IN !
	0 BEGIN
		/TIB OVER > IF
			KEY DUP 10 >
		ELSE
			FALSE
		THEN
	WHILE
		OVER TIB + C!
		1+
	REPEAT
	TIB >SOURCE !
	DUP /SOURCE !
	0=
;

: PARSE-NAME ( parse_name ) ( -- c-addr u )
	SOURCE >IN @ /STRING
	BEGIN OVER C@ BL > INVERT OVER 0> AND WHILE 1 /STRING REPEAT
	2DUP
	BEGIN OVER C@ BL > OVER 0> AND WHILE 1 /STRING REPEAT
	DUP >IN !
	NIP -
;
