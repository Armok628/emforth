// C\ #include <stdio.h>
// C\ #include "builtins/setraw.h"

key_code: /*: KEY ( key ) ;*/
	ASMLABEL(key_code);
	PUSH(sp) = tos;
	setraw(true);
	tos = getchar();
	setraw(false);
	NEXT();

emit_code: /*: EMIT ( emit ) ;*/
	ASMLABEL(emit_code);
	putchar(tos);
	tos = POP(sp);
	NEXT();
