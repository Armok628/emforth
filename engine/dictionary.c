word_c: // WORD
	asm("word:");
{
	int n = 0;
	char c;
	do {
		c = rx_char();
		tx_char(c);
	} while (c <= ' ');
	do {
		((char *)dp)[n++] = c;
		c = rx_char();
		tx_char(c);
	} while (c > ' ');
	PUSH(sp) = (cell)dp;
	PUSH(sp) = n;
	NEXT();
}
	
type_c: // TYPE
	asm("type:");
{
	int n = POP(sp);
	char *s = (char *)POP(sp);
	while (n --> 0)
		tx_char(*(s++));
	NEXT();
}

cr_c: // CR
	tx_char('\r');
	tx_char('\n');
	NEXT();

find_c: // FIND
	asm("find:");
	static struct fthdef *latest = LAST_VOC;
{
	int n = POP(sp);
	char *s = (char *)POP(sp);
	struct fthdef *d;
	for (d = latest; d != NULL; d = d->prev) {
		int i;
		if (n != d->len)
			continue;
		for (i = 0; i < n; i++)
			if (s[i] != d->name[i])
				break;
		if (i == n)
			break;
	}
	PUSH(sp) = (cell)d;
	NEXT();
}
