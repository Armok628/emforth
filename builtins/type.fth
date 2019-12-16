: TYPE ( type ) ( c-addr u -- )
	BEGIN
		DUP 0>
	WHILE
		OVER C@ EMIT
		1 /STRING
	REPEAT
	2DROP
;
