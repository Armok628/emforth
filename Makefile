CFLAGS = -Os -g -Wall -Wextra

WORD_LOCS = builtins/*

a.out: main.c fthdef.h prims.c dict.c cfas.c
	$(CC) $(CFLAGS) $< -o $@

labeled: main.c fthdef.h prims.c dict.c cfas.c
	$(CC) $(CFLAGS) -DUSE_ASMLABELS $<

prims.c: $(PRIM_LOCS)
	find $(WORD_LOCS) | grep \\.c$$ | sed 's/.*/#include "&"/' > $@

dict.c: gen_dict.pl $(WORD_LOCS)
	perl $< $(WORD_LOCS) > $@

cfas.c: gen_cfas.pl dict.c
	perl $< dict.c > $@

.PHONY: clean cleaner cleanest
clean:
	rm -f prims.c dict.c cfas.c
cleaner: clean
	rm -f a.out
cleanest: cleaner
	rm -f rm .*~ *~ */.*~ */*~
