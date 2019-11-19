#!/usr/bin/perl
use strict;
use warnings;

my @stack=();
my $state;
my @line=();
my %ct=();
my %cfa=();
my %data=(); # Hash of string array refs

# Compilation primitive
sub comma ($) {
	my ($cell)=@_;
	push @{$data{$state}},$cell;
}
sub commaxt ($) {
	my ($word)=@_;
	comma("&$ct{$word}_def.cfa");
}

# Control flow primitives
sub markbw {
	push @stack,scalar @{$data{$state}};
}
sub markfw {
	markbw();
	comma('UNRESOLVED_MARK');
}
sub fwresolve {
	my $a=pop @stack;
	my $o=scalar(@{$data{$state}})-$a;
	${$data{$state}}[$a]="(void **)($o*sizeof(cell_t))";
}
sub bwresolve {
	my $o=pop(@stack)-scalar(@{$data{$state}});
	comma("(void **)($o*sizeof(cell_t))");
}

# Utilities
sub swap {
	my $b=pop @stack;
	my $a=pop @stack;
	push @stack, $b;
	push @stack, $a;
}

# Immediate words
my %imm = (
	'/*:' => sub {
		$state=shift @line;
		$ct{$state} = $line[1];
		$cfa{$state}="&&$ct{$state}_code";
		$data{$state}=[];
	},
	':' => sub {
		$state=shift @line;
		$ct{$state} = $line[1];
		$cfa{$state}="&&$ct{'DOCOL'}_code";
		$data{$state}=[];
	},
	';' => sub {
		commaxt('EXIT');
		undef $state;
	},
	';*/' => sub {
		undef $state;
	},
	'(' => sub {
		do {} while @line and shift @line ne ')';
	},
	')' => sub {
	},
	'POSTPONE' => sub {
		commaxt(shift @line);
	},
	'[\']' => sub {
		commaxt('DOLIT');
		commaxt(shift @line);
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
	#TODO More immediates
);

# Line interpreter
my $err = 0;
sub interp ($) {
	chomp;
	@line=split ' ',$_;
	while (@line) {
		my $word=shift @line;
		if ($imm{$word}) {
			$imm{$word}();
		} elsif ($state) {
			if ($word=~/^-?\d+$/) {
				commaxt('DOLIT');
				comma("(void **)$word");
			} elsif ($ct{$word}) {
				commaxt($word);
			} else {
				print STDERR "$word?\n";
				$err = 1;
			}
		}
	}
}

my @lines = (<>);
# Collect C tokens
for (@lines) {
	$ct{$1}=$2 while /: (\S+) \( (\S+) \)/g;
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
