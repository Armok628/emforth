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
		$cfa{$state}="&&$ct{$state}_code";
		$data{$state}=[];
	},
	':' => sub {
		$state=shift @line;
		$cfa{$state}="&&$ct{'DOCOL'}_code";
		$data{$state}=[];
	},
	';' => sub {
		commaxt('EXIT') if defined $state;
		undef $state;
	},
	';*/' => sub {
		undef $state;
	},
	'CONSTANT' => sub {
		$state=shift @line;
		$cfa{$state}="&&$ct{'DOCONST'}_code";
		$data{$state}=["(void **)".pop @stack];
		undef $state;
	},
	'VARIABLE' => sub {
		$state=shift @line;
		$cfa{$state}="&&$ct{'DOVAR'}_code";
		$data{$state}=["NULL"];
		undef $state;
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
		$imm{$word}(), next if exists $imm{$word};
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
				last if shift @line eq ":";
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
