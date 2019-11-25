#!/usr/bin/perl
use strict;
use warnings;

my @stack=();
my $latest;
my $state=0;
my @line=();
my %ct=();
my %cfa=();
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
sub comma ($) {
	my ($cell)=@_;
	push @{$data{$latest}},$cell;
}
sub commaxt ($) {
	my ($word)=@_;
	comma("&$ct{$word}_def.cfa");
}
sub swap {
	my ($a,$b)=splice @stack, -2;
	push @stack, $b, $a;
}
sub word {
	return shift @line;
}
sub define {
	my ($n,$c,@d)=@_;
	$latest=$n;
	$cfa{$n}=!$c?"&&$ct{$n}_code":"&&$ct{$c}_code";
	$data{$n}=@d?[@d]:[];
	$imm{$n}=0;
}

# Primitive compiler words
my %prim = (
	'/*:' => sub {
		define(word());
	},
	':' => sub {
		define(word(),'DOCOL');
		$state=1;
	},
	';' => sub {
		commaxt('EXIT');
		$state=0;
	},
	';*/' => sub {},
	'CONSTANT' => sub {
		define(word(),'DOCONST','(void **)'.pop @stack);
	},
	'VARIABLE' => sub {
		define(word(),'DOVAR','NULL');
	},
	'C{' => sub {
		my $w;
		my @ws;
		while (@line) {
			$w=word();
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
			last if word() eq ')';
		}
	},
	')' => sub {},
	'POSTPONE' => sub {
		commaxt(word());
	},
	'[\']' => sub {
		commaxt('DOLIT');
		commaxt(word());
	},
	'IF' => sub {
		commaxt('0BRANCH');
		markfw();
	},
	'ELSE' => sub {
		commaxt('0BRANCH');
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
		commaxt('0BRANCH');
		markfw();
		swap();
	},
	'REPEAT' => sub {
		commaxt('BRANCH');
		bwresolve();
		fwresolve();
	},
	'IMMEDIATE' => sub {
		$imm{$latest}=1;
	}
	#TODO More immediates
);

# Line interpreter
my $err = 0;
sub interp ($) {
	chomp;
	@line=split ' ',$_;
	while (@line) {
		my $word=word();
		$prim{$word}(), next if exists $prim{$word};
		if ($state) {
			if ($word=~/^-?\d+$/) {
				commaxt('DOLIT');
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
				last if word() eq ":";
			}
		} elsif ($word=~/^(-?\d+)$/) {
			push @stack, $word;
		} elsif (exists $cfa{$word}
				and $cfa{$word} eq "&&$ct{'DOVAR'}_code"
				and @line>0 and $line[0] eq '!') {
			$data{$word}=["(void **)".pop @stack];
		}
	}
}

my @lines = (<>);
# Collect C tokens
for (@lines) {
	$ct{$2}=$3 while /(:|CONSTANT|VARIABLE) (\S+) \( (\S+) \)/g;
}
# Interpret every line of input
&interp for @lines;
die if $err;

# Output dictionary links to file
my $last;
print "static struct primitive\n\t";
print join ",\n\t",map {"${_}_def"} sort values %ct, "latest";
print ";\n\n";
for (reverse sort keys %ct) {
	print <<"EOT";
static struct primitive $ct{$_}_def = {
	.prev = @{[$last?"&${last}_def":"NULL"]},
	.name = "$_",
	.namelen = @{[length.($imm{$_}?"|msb":"")]},
	// .cfa = $cfa{$_},
	.data = {@{[join ', ',@{$data{$_}}]}},
};
EOT
	$last=$ct{$_};
}
print <<"EOT"
static struct primitive latest_def = {
	.prev = @{[$last?"&${last}_def":"NULL"]},
	.name = "LATEST",
	.namelen = 6,
	// .cfa = &&doconst_code,
	.data = {(void **)&latest_def},
};
EOT
