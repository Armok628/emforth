// C\ #include <termios.h>
// C\ static struct termios term;

raw_code: /*: RAW ( raw ) ;*/
	ASMLABEL(raw_code);
	tcgetattr(STDIN_FILENO,&term);
	term.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&term);
	goto next;

unraw_code: /*: -RAW ( unraw ) ;*/
	ASMLABEL(unraw_code);
	tcgetattr(STDIN_FILENO,&term);
	term.c_lflag|=(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&term);
	goto next;
