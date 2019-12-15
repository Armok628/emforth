: /STRING ( shift_string ) ( c-addr u n -- c-addr u )
	TUCK - >R + R>
;
: WITHIN ( within ) ( n min max -- flag )
	ROT TUCK < >R > R> OR INVERT
;
: >DIGIT ( to_digit ) ( char -- n | -1 )
	DUP 48 57 WITHIN IF \ 0-9
		48 - EXIT
	THEN DUP 65 90 WITHIN IF \ A-F
		55 - EXIT
	THEN DUP 96 122 WITHIN IF \ a-f
		86 - EXIT
	THEN DROP -1
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
