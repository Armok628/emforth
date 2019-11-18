#!/usr/bin/perl
my @cfas;
/\.cfa = (.*?),/ and push @cfas, $1 for (<>);
print <<"EOT"
void *cfas[@{[scalar @cfas]}] = {
	@{[join ",\n\t", reverse @cfas]},
};
EOT
