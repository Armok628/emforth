// C\ #include <stdio.h>
// C\ #include "builtins/setraw.h"

key_code: /*: KEY ( key ) ;*/
	ASMLABEL(key_code);
	PUSH(sp) = tos;
	setraw(true);
	tos = getchar();
	setraw(false);
	next();

emit_code: /*: EMIT ( emit ) ;*/
	ASMLABEL(emit_code);
	putchar(tos);
	tos = POP(sp);
	next();
