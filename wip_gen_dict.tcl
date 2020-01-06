#!/usr/bin/tclsh
set stack [list]
interp alias {} push {} lappend ::stack
proc pop {{n 1}} {
	incr n -1
	set vals    [lrange   $::stack end-$n end]
	set ::stack [lreplace $::stack end-$n end]
	return $vals
}

foreach name $argv {
	set id [open $name]
	lappend words {*}[split [read $id]]
	close $id
}
if {![info exists words]} {
	set words [split [read stdin]]
}
proc word {} {
	set ::words [lassign $::words word]
	return $word
}

set state ""
proc compile {args} {lappend ::def($::state) {*}$args; return}
proc here {} {return [llength $::def($::state)]}
proc swap {} {push {*}[lreverse [pop 2]]}

proc directive {name body} {set ::directive($name) $body}

directive :        {set ::state [word]; compile DOCOL}
directive \;       {compile EXIT; set ::state ""}
directive POSTPONE {compile [word]}
directive LITERAL  {compile "DOLIT" [pop]}
directive \[']     {compile "DOLIT" &[word]}
directive \[CHAR]  {compile "DOLIT" "'[string index [word] 0]'"}

proc mark> {} {push [here]; compile "UNRESOLVED"}
proc >resolve {} {set i [pop]; lset ::def($::state) $i [expr {[here]-$i}]}
proc mark< {} {push [here]}
proc <resolve {} {compile [expr {[pop]-[here]}]}

directive IF   {compile "0BRANCH"; mark>}
directive ELSE {compile  "BRANCH"; mark>; swap; >resolve}
directive THEN {>resolve}

directive BEGIN  {mark<}
directive WHILE  {compile "0BRANCH"; mark>; swap}
directive REPEAT {compile  "BRANCH"; <resolve; >resolve}
directive AGAIN  {compile  "BRANCH"; <resolve}
directive UNTIL  {compile "0BRANCH"; <resolve}

directive ? {while {[word] ne ":"} continue} ;# Ignore C ternary operators
directive ( {while {[word] ne ")"} continue}

directive /*: {set ::state [word]}
directive \;*/ {set ::state ""}

directive "" continue
while {[llength $words]} {
	set word [word]
	if {[info exists directive($word)]} {
		eval $directive($word)
	} elseif {$state ne ""} {
		if {[string is double $word]} {
			compile "DOLIT"
		}
		compile $word
	} elseif {[string is entier $word]} {
		push $word
	}
}

proc safename {name} {
	# Generate a reasonable C token from a given Forth name
	if {[string is entier $name]} {return $name}
	set name [string tolower $name]
	set name [string map [list \
	! _exclam_	\" _quote_	\# _pound_	\
	\$ _dollar_	% _pcent_	& _and_		\
	' _tick_	( _lparen_	) _rparen_	\
	* _star_	+ _plus_	, _comma_	\
	- _dash_	. _dot_		/ _slash_	\
	0 _zero_	1 _one_		2 _two_		\
	: _colon_	\; _semi_			\
	< _less_	= _equal_	> _more_	\
	? _quest_	@ _at_				\
	\[ _lbrkt_	\\ _bkslsh_	\] _rbrack_	\
	^ _caret_	` _grave_			\
	\{ _lbrace_	| _pipe_	\} _rbrace_	\
	~ _tilde_					\
	] $name]
	set name [string map [list "__" "_"] $name]
	return [string trim $name "_"]
}

proc xt {name} {
	if {[string is entier $name]} {
		return "(void **)$name"
	} else {
		return "&[safename $name]_def.cf"
	}
}

set ::prev "NULL"
proc putlink {name} {
	set def [lassign $::def($name) cf]
	if {[info exists ::def($cf)]} {
		set cf [xt [lindex $::def($cf) 0]]
	}
	set ctok [safename $name]
	puts "static struct fthdef ${ctok}_def = {
	.prev = $::prev,
	.name = \"$name\",
	.namelen = [string length $name],
	// .cf = $cf,
	.data = {[join [lmap w $def {xt $w}] ", "]},
}"
	set ::prev "&${ctok}_def"
}

foreach name [lsort -decreasing [array names def]] {
	putlink $name
}

# TODO: Revise builtins/* to accept new format
