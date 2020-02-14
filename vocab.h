#ifndef VOCAB_H
#define VOCAB_H
/*
 *	Tools for generating the Forth vocabulary
 */

struct fthdef {
	struct fthdef *prev;
	char *name;
	bool imm : 1;
	cell len : 8*sizeof(cell)-1;
	void *cf;
	void **data[];
};

// Used in the VOCAB X macro below to generate fthdef structs.
// The line filling in .cf is commented out because its value should be local to engine.
// It should be filled in when engine is called with a NULL instruction pointer.
#define DEF(cn,pr,nm,cf,...) \
struct fthdef cn = { \
	.prev = pr, \
	.name = nm, \
	.imm = 0, \
	.len = COUNT(nm), \
	/*.cf = cf,*/ \
	.data = {__VA_ARGS__}, \
};

// Used in the VOCAB X macro below to generate a list of code fields.
#define CF(cn,pr,nm,cf,...) cf,

// Used in the varargs of a VOCAB entry to compile code by hand.
#define XT(cn) &cn##_d.cf
#define LIT(n) XT(dolit),(void **)(n)

// VOCAB is an X-macro containing a list of Forth definitions.
// Definition ID's are postfixed with "_d" to avoid collision.
// Likewise, labels in the engine are postfixed with "_c".
#define VOCAB(X) \
	X(bye_d,NULL,"BYE",&&bye_c) \
\
	X(docol_d,&bye_d,"DOCOL",&&docol_c) \
	X(dolit_d,&docol_d,"DOLIT",&&dolit_c) \
\
	X(exit_d,&dolit_d,"EXIT",&&exit_c) \
	X(execute_d,&exit_d,"EXECUTE",&&execute_c) \
	X(branch_d,&execute_d,"BRANCH",&&branch_c) \
	X(qbranch_d,&branch_d,"?BRANCH",&&qbranch_c) \
\
	X(spfetch_d,&qbranch_d,"SP@",&&spfetch_c) \
	X(spstore_d,&spfetch_d,"SP!",&&spstore_c) \
	X(dup_d,&spstore_d,"DUP",&&dup_c) \
	X(drop_d,&dup_d,"DROP",&&drop_c) \
	X(swap_d,&drop_d,"SWAP",&&swap_c) \
	X(over_d,&swap_d,"OVER",&&over_c) \
\
	X(rpfetch_d,&over_d,"RP@",&&rpfetch_c) \
	X(rpstore_d,&rpfetch_d,"RP!",&&rpstore_c) \
	X(to_r_d,&rpstore_d,">R",&&to_r_c) \
	X(r_fetch_d,&to_r_d,"R@",&&r_fetch_c) \
	X(r_from_d,&r_fetch_d,"R>",&&r_from_c) \
\
	X(store_d,&r_from_d,"!",&&store_c) \
	X(fetch_d,&store_d,"@",&&fetch_c) \
	X(cstore_d,&fetch_d,"C!",&&cstore_c) \
	X(cfetch_d,&cstore_d,"C@",&&cfetch_c) \
\
	X(add_d,&cfetch_d,"+",&&add_c) \
	X(sub_d,&add_d,"-",&&sub_c) \
	X(and_d,&sub_d,"AND",&&and_c) \
	X(or_d,&and_d,"OR",&&or_c) \
	X(xor_d,&or_d,"XOR",&&xor_c) \
	X(zlt_d,&xor_d,"0<",&&zlt_c) \

// LAST_VOC is needed for the engine to know where to start filling in code fields
#define LAST_VOC &zlt_d

#endif
