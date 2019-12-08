#!/usr/bin/perl
my @cfs;
/\.cf = (.*?),/ and push @cfs, $1 for (<>);
print <<"EOT"
void *cfs[@{[scalar @cfs]}] = {
	@{[join ",\n\t", reverse @cfs]},
};
EOT
