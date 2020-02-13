CC = clang
CFLAGS = -Os -g

a.out: main.c *.h
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f a.out .*~ *~
