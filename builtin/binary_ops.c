#define OP2(n,a,b) \
n##_code: ASMLABEL(n##_code); \
	tos = b(POP(sp) a tos); \
	goto next;
OP2(add,+,) /*: + ( add ) ;*/
OP2(sub,-,) /*: - ( sub ) ;*/
OP2(mul,*,) /*: * ( mul ) ;*/
OP2(div,/,) /*: / ( div ) ;*/
OP2(mod,%,) /*: MOD ( mod ) ;*/
OP2(lsh,<<,) /*: LSHIFT ( lsh ) ;*/
OP2(rsh,>>,) /*: RSHIFT ( rsh ) ;*/

OP2(gt,>,-) /*: > ( gt ) ;*/
OP2(gte,>=,-) /*: >= ( gte ) ;*/
OP2(lt,<,-) /*: < ( lt ) ;*/
OP2(lte,<=,-) /*: <= ( lte ) ;*/
OP2(eq,==,-) /*: = ( eq ) ;*/
OP2(neq,!=,-) /*: <> ( neq ) ;*/

divmod_code: /*: /MOD ( divmod ) ;*/
	ASMLABEL(divmod_code);
	wp = (void **)sp[-1];
	sp[-1] = (cell_t)wp % tos;
	tos =  (cell_t)wp / tos;
	goto next;
