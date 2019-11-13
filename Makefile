CFLAGS = -Os -g -Wall -Wextra

WORD_SRCS = main.c

a.out: main.c fthdef.h dict.c cfas.c
	$(CC) $(CFLAGS) $< -o $@

labeled: main.c fthdef.h dict.c cfas.c
	$(CC) $(CFLAGS) -DUSE_ASMLABELS $<

cfas.c:
dict.c: phase1.pl $(WORD_SRCS)
	./$< $(WORD_SRCS)

.PHONY: clean cleaner cleanest
clean:
	rm -f dict.c cfas.c
cleaner: clean
	rm -f a.out
cleanest: cleaner
	rm -f .*~ *~
