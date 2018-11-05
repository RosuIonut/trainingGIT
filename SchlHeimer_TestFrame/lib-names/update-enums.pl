#!/usr/bin/perl

# This script takes a template file as input which has a special syntax:
# // INSERT enum "<enum name>" from "<full path>"
#
# These lines are replaced by the enumeration found in the specified path. This
# requires a pattern for enum definitions that is recognized (see regular
# expressions in sub insert_enum).
#
# Note that this script does not respect namespaces. Thus the enumeration items
# will appear on whatever level is specified in the template file.

use strict;
use POSIX qw(strftime);

my $input_file = "target-enums.in";
my $output_file = "target-enums.hpp";

my $enums_searched = 0;
my $enums_found = 0;

sub insert_enum
{
	$enums_searched++;
	my $enum = $_[0];
	my $path = $_[1];
	
	open(TARGET_FILE, "< $path") or die "cannot open $path as input";
	my @target_file = <TARGET_FILE>;
	close(TARGET_FILE);
	
	print OUTPUT_FILE "// Enum \"$enum\" from \"$path\"\n";
	
	my $in_enum = 0;
	my $found = 0;
	my $in_typedef = 0;
	my @typedef;
	foreach my $line(@target_file)
	{
		# Regular enums like
		# enum blah {
		# ...
		# };
		if ($line =~ m/enum\s+$enum\s*{?$/)
		{
			$in_enum = 1;
			$enums_found++;
			$found = 1;
		}
		if ($in_enum == 1)
		{
			print OUTPUT_FILE $line;
		}
		if ($in_enum == 1)
		{
			if ($line =~ m/}\s*;/ or $line =~ m/}\s*\w+\s*;/)
			{
				$in_enum = 0;
			}
		}
		
		# Typedefed enums like
		# typedef enum {
		# ...
		# } blah;
		if ($line =~ m/typedef\s+enum/)
		{
			$in_typedef = 1;
			@typedef=();
		}
		if ($in_typedef == 1)
		{
			push(@typedef, $line);
		}
		if ($in_typedef == 1)
		{
			if ($line =~ m/\}\s*$enum\s*;/)
			{
				print OUTPUT_FILE @typedef;
				$enums_found++;
				$found = 1;
			}
			if ($line =~ m/\}.*;/)
			{
				$in_typedef = 0;
			}
		}
	}
	
	if ($found == 0)
	{
		print "FAILED for enum \"$enum\" in \"$path\"\n";
	}
}

sub main
{
	print "===== Enum updater version 1.0 =====\n";

	open(INPUT_FILE, "< $input_file") or die "cannot open $input_file as input";
	my @target_enums = <INPUT_FILE>;
	close(INPUT_FILE);

	open(OUTPUT_FILE, "> $output_file") or die "cannot open $output_file as output";
	my $in_file = 0;
	foreach my $line(@target_enums)
	{
		my $skip = 0;
		
		if ($line =~ m/\/\/ GENERATED(\s+):/)
		{
			my $now = strftime "%Y-%m-%d %H:%M:%S", localtime;
			print OUTPUT_FILE "// GENERATED$1: $now\n";
			$skip = 1;
		}
	
		if ($line =~ m/\/\/ INPUT(\s+):/)
		{
			my $now = strftime "%Y-%m-%d %H:%M:%S", localtime;
			print OUTPUT_FILE "// INPUT$1: $input_file\n";
			$skip = 1;
		}
	
		if ($line =~ m/\/\/ INSERT enum \"([^"]*)\"\s+from\s+\"([^"]*)\"/)
		{       
			insert_enum($1, $2);
			$skip = 1;
		}
		
		if ($skip == 0)
		{
			print OUTPUT_FILE $line;
		}
	}
	close(OUTPUT_FILE);

	print "Found $enums_found of $enums_searched enumerations.\n";
	print "====== Enum updater finished =======\n";
}

main();