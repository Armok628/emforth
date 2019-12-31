: NUMBER-CHAR ( number_char ) ( c-addr u -- c-addr u 0 | char -1 )
	DUP 3 <> IF FALSE EXIT THEN
	OVER DUP 2 +
	C@ [CHAR] ' = SWAP
	C@ [CHAR] ' = AND IF
		DROP NIP
		1+ C@
		TRUE
	ELSE
		FALSE
	THEN
;

: NUMBER-BASE ( number_base ) ( c-addr u -- c-addr u u )
	OVER C@ >R 1 /STRING
	R@ [CHAR] $ = IF
		16
	ELSE R@ [CHAR] # = IF
		10
	ELSE R@ [CHAR] % = IF
		2
	ELSE
		-1 /STRING
		BASE @
	THEN THEN THEN
	R> DROP
;

: NUMBER-SIGN ( number_sign ) ( c-addr u -- c-addr u flag )
	OVER C@ [CHAR] - = IF
		1 /STRING TRUE
	ELSE
		FALSE
	THEN
;

: NUMBER ( number ) ( c-addr -- n -1 | c-addr 0 )
	DUP C@ 0= IF FALSE EXIT THEN
	DUP COUNT

	NUMBER-CHAR IF NIP TRUE EXIT THEN
	BASE @ >R NUMBER-BASE BASE !
	NUMBER-SIGN >R

	0 DUP 2SWAP >NUMBER NIP NIP IF
		DROP
		R> DROP
		FALSE
	ELSE
		NIP
		R> IF NEGATE THEN
		TRUE
	THEN
	R> BASE !
;
