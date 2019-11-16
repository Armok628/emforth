write_file_code: /*: WRITE-FILE ( write_file ) ;*/
	ASMLABEL(write_file_code);
	tos = write(tos, (char *)sp[-2], sp[-1]);
	sp -= 2;
	if (tos < 0)
		tos = errno;
	else
		tos = 0;
	goto next;

read_file_code: /*: READ-FILE ( read_file ) ;*/
	ASMLABEL(read_file_code);
	tos = read(tos, (char *)sp[-2], sp[-1]);
	sp -= 2;
	PUSH(sp) = tos;
	if (tos < 0)
		tos = errno;
	else
		tos = 0;
	goto next;
