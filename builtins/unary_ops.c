#define OP1(n,a,b) \
n##_code: ASMLABEL(n##_code); \
	tos = a(tos b); \
	NEXT();

OP1(inc,,+1) /*: 1+ ( inc ) ;*/
OP1(dec,,-1) /*: 1- ( dec ) ;*/
OP1(neg,-,) /*: NEGATE ( neg ) ;*/
OP1(not,~,) /*: INVERT ( not ) ;*/
OP1(lsh1,,<<1) /*: 2* ( lsh1 ) ;*/
OP1(rsh1,,>>1) /*: 2/ ( rsh1 ) ;*/

OP1(gtz,-,>0) /*: 0> ( gtz ) ;*/
OP1(gtez,-,>=0) /*: 0>= ( gtez ) ;*/
OP1(ltz,-,<0) /*: 0< ( ltz ) ;*/
OP1(ltez,-,<=0) /*: 0<= ( ltez ) ;*/
OP1(eqz,-,==0) /*: 0= ( eqz ) ;*/
OP1(neqz,-,!=0) /*: 0<> ( neqz ) ;*/

// : CELL+ ( cell_plus ) CELL + ;
// : CELLS ( cells ) CELL * ;
