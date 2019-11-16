CFLAGS = -Os -g -Wall -Wextra

WORD_LOCS = main.c builtin/*
PRIM_LOCS = builtin/*.c

a.out: main.c fthdef.h prims.c dict.c cfas.c
	$(CC) $(CFLAGS) $< -o $@

labeled: main.c fthdef.h dict.c cfas.c
	$(CC) $(CFLAGS) -DUSE_ASMLABELS $<

prims.c: $(PRIM_LOCS)
	for f in $^; do echo "#include \"$$f\"" >> $@; done

cfas.c:
dict.c: phase1.pl $(WORD_LOCS)
	perl $< $(WORD_LOCS)

.PHONY: clean cleaner cleanest
clean:
	rm -f prims.c dict.c cfas.c
cleaner: clean
	rm -f a.out
cleanest: cleaner
	rm -f rm .*~ *~ */.*~ */*~
