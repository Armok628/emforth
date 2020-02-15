CC = clang
CFLAGS = -Os -g
SRCS = engine/*.c

a.out: main.c engine.c vocab.h fthutil.h
	$(CC) $(CFLAGS) $< -o $@

vocab.h: genvoc $(SRCS)
	./genvoc $(SRCS) > $@

engine.c: $(SRCS)
	find $(SRCS) | sed 's/.*/#include "&"/' > $@

.PHONY: clean cleanest
clean:
	rm -f a.out engine.c vocab.h
cleanest: clean
	rm -f .*~ *~ */.*~ */*~
