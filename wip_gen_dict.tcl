#!/usr/bin/tclsh
set stack [list]
interp alias {} push {} lappend ::stack
proc pop {{n 1}} {
	incr n -1
	set vals    [lrange   $::stack end-$n end]
	set ::stack [lreplace $::stack end-$n end]
	return $vals
}

set state ""
set last ""
proc word {} {set ::words [lassign $::words word]; return $word}
proc compile {args} {lappend ::def($::state) {*}$args; return}
proc here {} {return [llength $::def($::state)]}
proc swap {} {push {*}[lreverse [pop 2]]}

proc cast {n} {return "(void **)$n"}
proc cells {n} {cast "($n*sizeof(cell_t))"}

proc mark> {} {push [here]; compile "UNRESOLVED"}
proc >resolve {} {set i [pop]; lset ::def($::state) $i [cells [expr {[here]-$i}]]}
proc mark< {} {push [here]}
proc <resolve {} {compile [cells [expr {[pop]-[here]}]]}

proc cid {name} {
	# Generate a reasonable C identifier from a given Forth name
	if {[string is entier $name]} {return $name}
	if {$name eq "-"} {return "minus"}
	set name [string tolower $name]
	set name [string map [list \
	 ! _store_	\" _quote_	\# _numsign_    \
	\$ _dollar_	 % _percent_	 & _and_	\
	 ' _tick_	 ( _lparen_	 ) _rparen_	\
	 * _star_	 + _plus_	 , _comma_	\
	 - _dash_	 . _dot_	 / _slash_	\
	 0 _zero_	 1 _one_	 2 _two_	\
	 : _colon_	\; _semicolon_			\
	<> _unequal_					\
	 < _less_	 = _equal_	 > _greater_	\
	 ? _question_	 @ _fetch_			\
	\[ _lbrack_	\\ _back_	\] _rbrack_	\
	 ^ _caret_	 ` _grave_			\
	\{ _lbrace_	 | _pipe_	\} _rbrace_	\
	 ~ _tilde_					\
	] $name]
	set name [string map [list "__" "_"] $name]
	return [string trim $name "_"]
}
proc xt {name} {return "&[cid $name].cf"}
proc cf {name} {lindex $::def($name) 0}

proc dir {name def} {set ::directive($name) $def}
dir :         {set ::state [word]; compile [cf DOCOL]}
dir \;        {compile [xt EXIT]; set ::last $::state; set ::state ""}
dir POSTPONE  {compile [xt [word]]}
dir LITERAL   {compile [xt DOLIT] [cast [pop]]}
dir \[']      {compile [xt DOLIT] [xt [word]]}
dir \[CHAR]   {compile [xt DOLIT] [cast [scan [string index [word] 0] %c]]}
dir IMMEDIATE {set ::flags($::last) "|MSB"}

dir IF   {compile [xt 0BRANCH]; mark>}
dir ELSE {compile  [xt BRANCH]; mark>; swap; >resolve}
dir THEN {>resolve}

dir BEGIN  {mark<}
dir WHILE  {compile [xt 0BRANCH]; mark>; swap}
dir REPEAT {compile  [xt BRANCH]; <resolve; >resolve}
dir AGAIN  {compile  [xt BRANCH]; <resolve}
dir UNTIL  {compile [xt 0BRANCH]; <resolve}

dir CONSTANT {set ::def([word]) [list [cf DOCON] [pop]]}
dir VARIABLE {set ::def([word]) [list [cf DOVAR] 0]}

dir ? {while {[word] ne ":"} continue} ;# Ignore C ternary operators
dir ( {while {[word] ne ")"} continue}

dir {} {continue}

foreach name $argv {lappend words {*}[split [read [open $name]]]}
if {![info exists words]} {set words [split [read stdin]]}
foreach {m f c} [regexp -all -inline {/\*: (.*?) (.*?) {;\*/}} $words] {
	set ::def($f) "&&$c"
}

while {[llength $words]} {
	set word [word]
	if {[info exists directive($word)]} {
		eval $directive($word)
	} elseif {$state ne ""} {
		if {[string is entier $word]} {
			compile [xt DOLIT] [cast $word]
		} else {
			compile [xt $word]
		}
	} elseif {[info exists def($word)] && [lindex $def($word) 0] eq [cf DOVAR]} {
		lset def($word) 1 [pop]
	} elseif {[string is entier $word]} {
		push $word
	}
}

proc flags {name} {expr {[info exists ::flags($name)] ? $::flags($name) : ""}}
puts "static struct fthdef\n\t[join [lmap n [lsort [array names def]] {cid $n}] ",\n\t"];\n"
set ::prev "NULL"
foreach name [lsort -decreasing [array names def]] {
	set params [lassign $::def($name) cf]
	puts "static struct fthdef [cid $name] = {
	.prev = $::prev,
	.name = \"$name\",
	.namelen = [string length $name][flags $name],
	// .cf = $cf,
	.data = {[join $params ", "]},
}"
	set ::prev "&[cid $name]"
}
