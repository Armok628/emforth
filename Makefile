CC = clang
CFLAGS = -Os -g
SRCS = engine/*.c

a.out: main.c engine.c *.h
	$(CC) $(CFLAGS) $< -o $@

engine.c: $(SRCS)
	find $(SRCS) | sed 's/.*/#include "&"/' > $@

.PHONY: clean cleanest
clean:
	rm -f a.out engine.c
cleanest:
	rm -f .*~ *~ */.*~ */*~
