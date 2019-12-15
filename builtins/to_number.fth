: /STRING ( shift_string ) ( c-addr u n -- c-addr u )
	TUCK - >R + R>
;
: WITHIN ( within ) ( n min max -- flag )
	ROT TUCK < >R > R> OR INVERT
;
: ?EXIT ( qexit ) ( flag ) IF R> DROP THEN ;
: >DIGIT ( to_digit ) ( char -- u | -1 )
	48 - DUP 0 9 WITHIN ?EXIT \ 0-9
	7 - DUP 10 35 WITHIN ?EXIT \ A-Z
	32 - DUP 10 35 WITHIN ?EXIT \ a-z
	DROP -1
;
: >NUMBER ( to_number ) ( ud c-addr u -- ud c-addr u )
	BEGIN
		DUP 0>
	WHILE
		OVER C@ >DIGIT
		DUP 0 BASE @ 1- WITHIN 0= IF
			DROP EXIT
		THEN
		-ROT 2>R >R
		BASE @ TUCK
		2>R M* DROP
		2R> M* ROT + SWAP
		R> M+ 2R>
		1 /STRING
	REPEAT
;
