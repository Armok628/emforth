// C\ #include "builtins/file_access.h"

// C{ RO } CONSTANT R/O ( ro )
// C{ WO } CONSTANT W/O ( wo )
// C{ RW } CONSTANT R/W ( rw )
// : BIN ( bin ) 8 OR ;

open_file_code: /*: OPEN-FILE ( open_file ) ;*/
	ASMLABEL(open_file_code);
	tos = (cell_t)open_file((char *)sp[-2], (size_t)sp[-1], (enum fam)tos);
	PUSH(sp) = tos;
	tos = ior;
	NEXT();

create_file_code: /*: CREATE-FILE ( create_file ) ;*/
	ASMLABEL(create_file_code);
	tos = (cell_t)create_file((char *)sp[-2], (size_t)sp[-1], (enum fam)tos);
	PUSH(sp) = tos;
	tos = ior;
	NEXT();

close_file_code: /*: CLOSE-FILE ( close_file ) ;*/
	ASMLABEL(close_file_code);
	tos = close_file((fileid)tos);
	NEXT();

read_file_code: /*: READ-FILE ( read_file ) ;*/
	ASMLABEL(read_file_code);
	tos = read_file((void *)sp[-2], (size_t)sp[-1], (fileid)tos);
	PUSH(sp) = tos;
	tos = ior;
	NEXT();

write_file_code: /*: WRITE-FILE ( write_file ) ;*/
	ASMLABEL(write_file_code);
	tos = write_file((void *)sp[-2], (size_t)sp[-1], (fileid)tos);
	PUSH(sp) = tos;
	tos = ior;
	NEXT();
