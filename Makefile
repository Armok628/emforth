CC = clang
CFLAGS = -Os -g -Wall -Wextra

WORD_LOCS = builtins/*.c builtins/*.fth

a.out: main.c fthdef.h cell.h prims.c dict.c cfs.c
	$(CC) $(CFLAGS) $< -o $@

labeled: main.c fthdef.h cell.h prims.c dict.c cfs.c
	$(CC) $(CFLAGS) -DUSE_ASMLABELS $<

prims.c: $(WORD_LOCS)
	find $(WORD_LOCS) | grep \\.c$$ | sed 's/.*/#include "&"/' > $@

dict.c: gen_dict.pl $(WORD_LOCS)
	./$< $(WORD_LOCS) > $@

cfs.c: gen_cfs.pl dict.c
	./$< dict.c > $@

.PHONY: clean cleaner cleanest
clean:
	rm -f prims.c dict.c cfs.c
cleaner: clean
	rm -f a.out
cleanest: cleaner
	rm -f rm .*~ *~ */.*~ */*~
