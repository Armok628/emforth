VARIABLE BASE ( base )
10 BASE !

: /STRING ( shift_string ) ( c-addr u n -- c-addr u )
	TUCK - >R + R>
;
: WITHIN ( within ) ( n min max -- flag )
	ROT TUCK < >R > R> OR INVERT
;
: >DIGIT ( to_digit ) ( char -- u | -1 )
	48 - DUP 0 9 WITHIN IF EXIT THEN \ 0-9
	7 - DUP 10 35 WITHIN IF EXIT THEN \ A-Z
	32 - DUP 10 35 WITHIN IF EXIT THEN \ a-z
	DROP -1
;
: >NUMBER ( to_number ) ( ud c-addr u -- ud c-addr u )
	BEGIN
		DUP 0>
	WHILE
		OVER C@ >DIGIT
		DUP 0 BASE @ 1- WITHIN 0= IF
			DROP EXIT
		THEN ( ud c-addr u char )
		-ROT 2>R >R
		BASE @ TUCK
		M* DROP >R M* R> +
		R> M+
		2R> 1 /STRING
	REPEAT
;
