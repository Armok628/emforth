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

directive ( {while {[word] != ")"} continue}

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
	} elseif {[string is double $word]} {
		push $word
	}
}

foreach name [array names def] {
	puts "$name => $def($name)]"
}

# 	TODO
# * Handle primitives defined in C with the words /*: and ;*/
# * Define output procedures for dictionary links
