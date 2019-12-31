#!/usr/bin/perl
use strict;
use warnings;

my @stack=();
my $latest;
my $state=0;
my @line=();
my %ct=();
my %cf=();
my %imm=();
my %data=(); # Hash of string array refs

# Control flow primitives
sub markbw {
	push @stack,scalar @{$data{$latest}};
}
sub markfw {
	markbw();
	comma('UNRESOLVED_MARK');
}
sub fwresolve {
	my $a=pop @stack;
	my $o=scalar(@{$data{$latest}})-$a;
	${$data{$latest}}[$a]="(void **)($o*sizeof(cell_t))";
}
sub bwresolve {
	my $o=pop(@stack)-scalar(@{$data{$latest}});
	comma("(void **)($o*sizeof(cell_t))");
}

# Utilities
sub comma {
	my ($cell)=@_;
	push @{$data{$latest}},$cell;
}
sub commaxt {
	my ($word)=@_;
	comma("&$ct{$word}_def.cf");
}
sub swap {
	my ($a,$b)=splice @stack, -2;
	push @stack, $b, $a;
}
sub define {
	my ($n,$c,@d)=@_;
	$latest=$n;
	$cf{$n}="&&$ct{$c?$c:$n}_code";
	$data{$n}=@d?[@d]:[];
	$imm{$n}=0;
}

# Primitive compiler words
my %prim;
%prim = (
	'/*:' => sub {
		define(shift @line);
	},
	':' => sub {
		define(shift @line,'DOCOL');
		$state=1;
	},
	';' => sub {
		commaxt('EXIT');
		$state=0;
	},
	';*/' => sub {},
	'CONSTANT' => sub {
		define(shift @line,'DOCON','(void **)'.pop @stack);
	},
	'VARIABLE' => sub {
		define(shift @line,'DOVAR','NULL');
	},
	'C{' => sub {
		my $w;
		my @ws;
		while (@line) {
			$w=shift @line;
			last if $w eq '}';
			push @ws,$w;
		}
		push @stack,"@ws";
	},
	'}' => sub {},
	'C\\' => sub {
		print "@line\n";
		@line = ();
	},
	'\\' => sub {
		@line = ();
	},
	'(' => sub {
		while (@line) {
			last if shift @line eq ')';
		}
	},
	')' => sub {},
	'POSTPONE' => sub {
		commaxt(shift @line);
	},
	'[\']' => sub {
		commaxt('LIT');
		commaxt(shift @line);
	},
	'IF' => sub {
		commaxt('0BRANCH');
		markfw();
	},
	'ELSE' => sub {
		commaxt('BRANCH');
		markfw();
		swap();
		fwresolve();
	},
	'THEN' => sub {
		fwresolve();
	},
	'BEGIN' => sub {
		markbw();
	},
	'AGAIN' => sub {
		commaxt('BRANCH');
		bwresolve();
	},
	'UNTIL' => sub {
		commaxt('0BRANCH');
		bwresolve();
	},
	'WHILE' => sub {
		$prim{'IF'}();
		swap();
	},
	'REPEAT' => sub {
		$prim{'AGAIN'}();
		fwresolve();
	},
	'IMMEDIATE' => sub {
		$imm{$latest}=1;
	},
	'LITERAL' => sub {
		commaxt('LIT');
		comma('(void **)'.pop @stack);
	},
	'[CHAR]' => sub {
		my $char = (split //,shift @line)[0];
		push @stack, $char eq "'" ? "'\\$char'" : "'$char'";
		$prim{'LITERAL'}();
	}
	#TODO More immediates
);

# Line interpreter
my $err = 0;
sub interp ($) {
	chomp;
	@line=split ' ',$_;
	while (@line) {
		my $word=shift @line;
		$prim{$word}(), next if exists $prim{$word};
		if ($state) {
			if ($word=~/^-?\d+$/) {
				commaxt('LIT');
				comma("(void **)$word");
			} elsif ($ct{$word}) {
				commaxt($word);
			} else {
				print STDERR "$word?\n";
				$err = 1;
			}
			next;
		}
		if ($word eq "?") { # Ignore C ternary operators
			while (@line) {
				last if shift @line eq ":";
			}
		} elsif ($word=~/^(-?\d+)$/) {
			push @stack, $word;
		} elsif (exists $cf{$word}
				and $cf{$word} eq "&&$ct{'DOVAR'}_code"
				and @line>0 and $line[0] eq '!') {
			$data{$word}=['(void **)'.pop @stack];
		}
	}
}

my @lines = (<>);
push @lines, "VARIABLE FORTH-WORDLIST ( forth_wordlist ) C{ &forth_wordlist_def } FORTH-WORDLIST !";
# Collect C tokens
for (@lines) {
	$ct{$2}=$3 while /(:|CONSTANT|VARIABLE) (\S+) \( (\S+) \)/g;
}
# Interpret every line of input
&interp for @lines;
die if $err;

# Output dictionary links to file
my $last;
print "static struct fthdef\n\t";
print join ",\n\t",map {"${_}_def"} sort values %ct;
print ";\n\n";

sub print_def ($) {
	print <<"EOT";
static struct fthdef $ct{$_}_def = {
	.prev = @{[$last?"&${last}_def":"NULL"]},
	.name = "$_",
	.namelen = @{[length.($imm{$_}?"|MSB":"")]},
	// .cf = $cf{$_},
EOT
	print "\t.data = {@{[join ', ',@{$data{$_}}]}},\n" if (scalar @{$data{$_}});
	print "};\n";
	$last=$ct{$_};
}

for (reverse sort keys %ct) {
	&print_def if ($_ ne 'FORTH-WORDLIST');
}
$_='FORTH-WORDLIST'; &print_def; # Lazy hack
