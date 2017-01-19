#!/bin/perl
#  **********************************************************************
#
#  Copyright (c) 2003
#  StarMiddleware.net	 
#  www.StarMiddleware.net
#                        
#  All Rights Reserved   
#                           
# **********************************************************************

$top_srcdir=$ARGV[0];

while(<STDIN>)
{
    if(/\\$/)
    {
	chop;
	chop;
	$line = $line . $_;
    }
    else
    {
	$_ = $line . $_;
	
	($t, @d) = split;

	foreach (@d)
	{
	    if(!/^\//)
	    {
		if(substr($_, 0, length("$top_srcdir/../")) ne
		   "$top_srcdir/../")
		{
		    $map{$t} .= " $_";
		}
	    }
	}

	$line = ""
    }
}

open(UNIX, "> .depend") || die "Can't open .depend: $!";
open(WIN, "> .windep") || die "Can't open .windep: $!";

while(($key,$value) = each %map)
{
    print UNIX "$key$value\n";
    $key =~ s/\.o/.obj/;
    $value =~ s/\//\\/g;
    print WIN "$key$value\n";
}

close(UNIX);
close(WIN);
