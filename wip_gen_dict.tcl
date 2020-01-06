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

proc directive {name body} {set ::directive($name) $body}

directive :        {set ::state [word]; compile "DOCOL"}
directive \;       {compile EXIT; set ::last $::state; set ::state ""}
directive POSTPONE {compile [word]}
directive LITERAL  {compile "DOLIT" [pop]}
directive \[']     {compile "DOLIT" &[word]}
directive \[CHAR]  {compile "DOLIT" "'[string index [word] 0]'"}
directive IMMEDIATE {set ::flags($::last) "|MSB"}

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

proc cname {name} {
	# Generate a reasonable C token from a given Forth name
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

proc xt {name} {expr {[string is entier $name] ? "(void **)$name" : "&[cname $name].cf"}}
proc flags {name} {expr {[info exists ::flags($name)] ? $::flags($name) : ""}}
proc cf {name} {
	set cf [lindex $::def($name) 0]
	if {[info exists ::def($cf)]} {
		return "&&[lindex $::def($cf) 0]"
	} else {
		return "&&$cf"
	}
}

foreach name $argv {
	set id [open $name]
	lappend words {*}[split [read $id]]
	close $id
}
if {![info exists words]} {
	set words [split [read stdin]]
}

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

puts "static struct fthdef\n\t[join [lmap n [lsort [array names def]] {cname $n}] ",\n\t"];\n"
set ::prev "NULL"
foreach name [lsort -decreasing [array names def]] {
	set params [lassign $::def($name) cf]
	puts "static struct fthdef [cname $name] = {
	.prev = $::prev,
	.name = \"$name\",
	.namelen = [string length $name][flags $name],
	// .cf = [cf $name],
	.data = {[join [lmap w $params {xt $w}] ", "]},
}"
	set ::prev "&[cname $name]"
}
