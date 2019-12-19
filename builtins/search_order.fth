C\ struct fthdef forth_wordlist;
C\ struct fthdef **context[16] = {(struct fthdef **)&forth_wordlist.data[0]};
C{ &context } CONSTANT CONTEXT ( context )

VARIABLE #ORDER ( n_order )
0 #ORDER !

: GET-ORDER ( get_order ) ( -- widn..1 n )
	#ORDER @ >R
	0 BEGIN
		R@ <
	WHILE
		DUP CELLS CONTEXT + @ SWAP 1+
	REPEAT
	DROP
	R>
;

: SET-ORDER ( set_order ) ( wid1..n n -- )
	DUP -1 = IF
		DROP
		FORTH-WORDLIST CONTEXT !
		1 #ORDER !
		EXIT
	THEN
	>R
	0 BEGIN
		R@ <
	WHILE
		SWAP OVER CELLS CONTEXT + ! 1+
	REPEAT
	R> #ORDER !
;
