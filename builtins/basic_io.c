// C\ #include <stdio.h>
// C\ #include "builtins/setraw.h"

	ASMLABEL(key_code);
key_code: /*: KEY ( key ) ;*/
	PUSH(sp) = tos;
	setraw(true);
	tos = getchar();
	setraw(false);
	NEXT();

	ASMLABEL(emit_code);
emit_code: /*: EMIT ( emit ) ;*/
	putchar(tos);
	tos = POP(sp);
	NEXT();
