// C\ #include <unistd.h>
// C\ #include <errno.h>

write_file_code: /*: WRITE-FILE ( write_file ) ;*/
	ASMLABEL(write_file_code);
	tos = write(tos, (char *)sp[-2], sp[-1]);
	sp -= 2;
	tos = tos < 0 ? errno : 0;
	goto next;

read_file_code: /*: READ-FILE ( read_file ) ;*/
	ASMLABEL(read_file_code);
	tos = read(tos, (char *)sp[-2], sp[-1]);
	sp -= 2;
	PUSH(sp) = tos;
	tos = tos < 0 ? errno : 0;
	goto next;

// C\ #include <stdio.h>
// C\ #include <termios.h>
static struct termios term;

key_code: /*: KEY ( key ) ;*/
	ASMLABEL(key_code);
	PUSH(sp) = tos;
	tcgetattr(STDIN_FILENO,&term);
	term.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&term);
	tos = getchar();
	term.c_lflag|=(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&term);
	goto next;

emit_code: /*: EMIT ( emit ) ;*/
	ASMLABEL(emit_code);
	putchar(tos);
	tos = POP(sp);
	goto next;
