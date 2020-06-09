CC = clang
CFLAGS = -Os -g -Wall

SRCS = engine/*.c
SYS = sysif/linux.c

ALL = a.out engine.c vocab.h

all: $(ALL)

a.out: main.o sysif.o
	$(CC) $^ -o $@

main.o: main.c engine.c vocab.h fthutil.h
	$(CC) $(CFLAGS) -c $< -o $@

sysif.o: $(SYS) sysif/sysif.h
	$(CC) $(CFLAGS) -c $< -o $@

vocab.h: genvoc $(SRCS)
	./$< $(SRCS) > $@

engine.c: $(SRCS)
	find $(SRCS) | sed 's/.*/#include "&"/' > $@

.PHONY: clean
clean:
	rm -f $(ALL) *.o
