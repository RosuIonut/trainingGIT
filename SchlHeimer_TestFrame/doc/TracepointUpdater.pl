#!/usr/bin/perl

# This script creates a CSV file that describes the configured trace points.
#
# Input:
# - test specifications in MRTS format (directory with *.txt files)
# - test frame code (path to pkg directory)
# - trace point configuration (path to TraceConfig.cpp from TestFrameInterface)
# - optional: old CSV file from previous run
#
# Output:
# - newly generated CSV file
#
# When new trace points are found, the location is filled with the code file
# name (if the trace point was found in the code, of course), the Description
# column is left empty and the Arguments column is preset with the number of
# arguments.
# When an existing CSV file is used as input, the columns Location, Description
# and Arguments are copied into the output CSV, thus keeping the information.
# This makes it possible to describe the location in more detail (e.g. the
# exact method call) and add a custom description of trace point and arguments.
#
# The script also does some validation:
# - check if item types are used consistently within each spec
#   (e.g. detect if an item is used with both Happened and Changed)
# - find deactivated trace points in code (only simple checking for C++ style
#   comments, not for block comments or preprocessor switches)
# - find items which are used in the spec but that do not exist in config
# - find items which are configured but not referenced by any spec
# - find items which are configured but not referenced by code
# - find items of type X which are referenced by a spec as items of type Y
# - find items of type X which are referenced by the code as items of type Y
# - find items which are configured but did not exist in old CSV file
# - find items which exist in old CSV file but are not configured anymore

use strict;
use File::Find;

# Directory with MRTS files (must have extension *.txt)
my $spec_dir = "specs";
# Directory with target software ("pkg")
my $pkg_dir = "Q:/pkg";
# Path to TraceConfig.cpp that contains the configuration table
my $cfg_table_file = "../TestFrameInterface/TraceConfig.cpp";
# Name of old CSV file used as optional input
my $old_csv_file = "TracepointList.csv";
# Name of new CSV file used for generated output
my $new_csv_file = "new_TracepointList.csv";

##############################################################################

# Flag if old CSV file exists
my $has_old_csv = 0;
# Hash that contains trace points from all MRTS files
# 1. dim: spec name
# 2. dim: trace point name
# 3. dim: Count (number of occurences), Type (Event or Data)
my %spec_trace_points;
# Hash that contains trace points from target software
# 1. dim: trace point name
# 2. dim: Location (code file), Type (Event or Data)
my %code_trace_points;
# Hash that contains trace points from configuration table
# 1. dim: trace point name
# 2. dim: Type (Event or Data), ArgCount (number of arguments)
my %table_trace_points;
# Hash that contains trace points from old CSV file
# 1. dim: trace point name
# 2. dim: content of columns Type, Location, Description, Arguments
my %old_csv_points;

# Open all text files from spec directory and fill spec_trace_points hash
sub get_spec_trace_points
{
	my $dir = $_[0];
	
	# Run through directory
	opendir(DIR, $dir) or die "Cannot open $dir: $!";
	while (my $file = readdir(DIR)) {
		# Skip non-text files
		next if not ($file =~ m/\.txt$/);
		
		my $file_valid = 0;    # valid MRTSF marker found
		my $spec_name;         # spec name from Filenam: header
		my $in_block = 0;      # currently in Expected block

		# Read through file
		open(FILE, "< $dir\\$file") or die "Cannot open $file: $!";
		foreach my $line(<FILE>) {
			# Only check if not already found to be valid
			if ($file_valid == 0) {
				# Find Format: header
				if ($line =~ m/^Format:\s+(\w.*)$/) {
					# Check for "MRTSF 1" marker
					if ($1 eq "MRTSF 1") {
						$file_valid = 1;
					} else {
						print "Unknown format: $1\n";
						last;
					}
				}
				next;
			}
			# Check for Filename: header
			if ($line =~ m/Filename:\s+(\w.*)$/) {
				$spec_name = $1;
				# Use a shorter name if the file name matches our expectation
				$spec_name = $1 if ($spec_name =~ m/ITS_FPK_HMI_SW_System_(\w+).docm/);
				next;
			}
			# Check for beginning of Expected block
			if ($line =~ m/^Begin Expected$/) {
				$in_block = 1;
				next;
			}
			# Check for end of Expected block
			if ($line =~ m/^End Expected$/) {
				$in_block = 0;
				next;
			}
			# If block is entered, check for trace points
			if ($in_block == 1) {
				# Events are specified by Happened and NotHappened
				if ($line =~ m/^\s*(Not)?Happened\((\w+).*\)$/) {
					# Increase occurence counter
					$spec_trace_points{$spec_name}{$2}{Count}++;
					# Type of trace point is defined by first reference
					if ($spec_trace_points{$spec_name}{$2}{Count} == 1) {
						# Set type initially
						$spec_trace_points{$spec_name}{$2}{Type} = "Event";
					} else {
						# Check if previous set type matches
						if ($spec_trace_points{$spec_name}{$2}{Type} ne "Event") {
							$spec_trace_points{$spec_name}{$2}{Type} = "Inconsistent";
						}
					}
				}
				# Data items are specified by Changed and NotChanged
				if ($line =~ m/^\s*(Not)?Changed\((\w+).*\)$/) {
					# Increase occurence counter
					$spec_trace_points{$spec_name}{$2}{Count}++;
					# Type of trace point is defined by first reference
					if ($spec_trace_points{$spec_name}{$2}{Count} == 1) {
						# Set type initially
						$spec_trace_points{$spec_name}{$2}{Type} = "Data";
					} else {
						# Check if previous set type matches
						if ($spec_trace_points{$spec_name}{$2}{Type} ne "Data") {
							$spec_trace_points{$spec_name}{$2}{Type} = "Inconsistent";
						}
					}
				}
			}
		}
		close(FILE);
	}
	closedir(DIR);
}

# Callback function for Find(); searched for trace points in a code file
sub wanted
{
	my $file =$_;
	
	# Only work with files with extensions .c, .h, .cpp and .hpp
	if ($file =~ m/\.c$/ or $file =~ m/\.h$/ or $file =~ m/\.cpp$/ or $file =~ m/\.hpp$/) {
		my $linenr = 0;
		open(FILE, "< $_") or die "Cannot open $_: $!";
		foreach my $line(<FILE>) {
			$linenr++;
			# Check for TT_vTraceEvent and TT_vTraceData
			# Whitespace may preceed, everything else is considered as comment
			# or other means of deactivating the trace point
			if ($line =~ m/^\s*TT_vTrace(\w+)\(TT_(\w+)/) {
				# Store location and type of item in hash
				$code_trace_points{$2}{Location} = $_;
				$code_trace_points{$2}{Type} = $1;
			}
			# Check if there is a call to TT_vTrace*, and ...
			if ($line =~ m/TT_vTrace\w+\(TT_(\w+)/) {
				# ... check if it is not active, then ...
				if (not $line =~ m/^\s+TT_vTrace\w+\(TT_(\w+)/) {
					# ... print a warning
					print "Warning: deactivated trace point: $1 ($file:$linenr)\n";
				}
			}
		}
		close(FILE);
	}
}
# Small wrapper to call File::find
sub get_code_trace_points
{
	# Walk through source code directory
	find(\&wanted, $_[0]);
}

# Get trace point configuration from TestFrameInterface table
sub get_table_trace_points
{
	my $file = $_[0];
	my $in_table = 0;
	
	# Read through file
	open(FILE, "< $file") or die "Cannot open $file: $!";
	foreach my $line(<FILE>) {
		# Search for config table
		if ($line =~ m/const tstItemAttr astTraceItemTable\[\]/) {
			$in_table = 1;
			next;
		}
		# Skip line if not in table
		next if $in_table == 0;
		# "};" is considered as end of table
		last if ($line =~ m/};/);
		# Parse line to get item name, type and argument count
		if ($line =~ m/{ SHMEM_nenTrace_\w+,\s*\"([^"]+)",\s*n(\w+),\s*n\w+,\s*(\d)+,/) {
			$table_trace_points{$1}{Type} = $2;
			$table_trace_points{$1}{ArgCount} = $3;
		}
	}
	close(FILE);
}

# Get content of old CSV file (if existing)
sub read_csv_file
{
	my $file = $_[0];
	my $line_nr = 0;

	# Print a diagnostic warning of the old CSV file was not found
	if (not $has_old_csv) {
		print "Warning: old CSV file ($old_csv_file) doesn't exist. First time run?\n";
		return;
	}
	# Read through file
	open(FILE, "< $file") or die "Cannot open $file: $!";
	foreach my $line(<FILE>) {
		$line_nr++;
		# skip header line
		next if ($line_nr == 1);
		# Parse line to get item name, type, location, description and arguments
		if ($line =~ m/^(\w+);(\w+);([^;]*);([^;]*);([^;]*).*\n/) {
			# Store information in hash
			$old_csv_points{$1}{Type} = $2;
			$old_csv_points{$1}{Location} = $3;
			$old_csv_points{$1}{Description} = $4;
			$old_csv_points{$1}{Arguments} = $5;
		}
	}
	close(FILE);
}

# Write new CSV file as output
sub write_csv_file
{
	my $file = $_[0];
	
	open(FILE, "> $file") or die "Cannot open $file: $!";
	# Table header line
	print FILE "Name;Type;Location;Description;Arguments";
	# Header line continued: one column for each spec
	foreach my $spec(sort keys %spec_trace_points) {
		print FILE ";Used by $spec";
	}
	# Header line end
	print FILE "\n";
	# Run through all items from config table
	foreach my $name(sort keys %table_trace_points) {
		# local variables for column content
		my $type = $table_trace_points{$name}{Type};
		my $args = $table_trace_points{$name}{ArgCount};
		my $loc = "";
		my $desc = "";

		# insert "---" instead of "0" if no arguments exist
		$args = "---" if ($args == 0);
		# overwrite location if item exists in code
		$loc = $code_trace_points{$name}{Location} if (exists $code_trace_points{$name});
		# overwrite location if better description exists in old CSV
		$loc = $old_csv_points{$name}{Location} if (exists $old_csv_points{$name} and $old_csv_points{$name}{Location} ne "");
		# overwrite description if better description exists in old CSV
		$desc = $old_csv_points{$name}{Description} if (exists $old_csv_points{$name} and $old_csv_points{$name}{Description} ne "");
		# overwrite argument count with existing argument descriptions
		$args = $old_csv_points{$name}{Arguments} if (exists $old_csv_points{$name} and $old_csv_points{$name}{Arguments} ne "");
		
		# Write common entries
		print FILE "$name;$type;$loc;$desc;$args";
		# Set markers for references in test specs
		foreach my $spec(sort keys %spec_trace_points) {
			my $count = 0;
			# Get occurence counter
			if (exists $spec_trace_points{$spec}{$name}) {
				$count = $spec_trace_points{$spec}{$name}{Count};
			}
			# Mark is 'X' if item is referenced
			my $mark = $count > 0 ? "X" : "";
			# Switch this to get occurence counters instead of marks
			#print FILE ";$count";
			print FILE ";$mark";
		}
		print FILE "\n";
	}
	close(FILE);
}

# Find items which are used in the spec but that do not exist in config
sub find_unknown_items_used_by_spec()
{
	my $last_faulty_spec = "";
	# Run through all specifications
	foreach my $spec(sort keys %spec_trace_points) {
		# Run through all items in each spec
		foreach my $item(sort keys %{$spec_trace_points{$spec}}) {
			# Check if that item also exists in configuration
			if (not exists $table_trace_points{$item}) {
				# Doesn't exist => unknown item
				if ($spec ne $last_faulty_spec) {
					# Separator if there was already output
					print "\n" if ($last_faulty_spec ne "");
					# Header printed only once
					print "Unknown items referenced by \"$spec\":\n";
					$last_faulty_spec = $spec;
				}
				# Print unknown item
				print "\t$item\n";
			}
		}
	}
	# Separator from next block, only if there was output
	print "\n" if ($last_faulty_spec ne "");
}

# Find items which are configured but not referenced by any spec
sub find_configured_items_not_referenced_by_any_spec()
{
	my $output = 0;
	# Run through all configured trace items
	foreach my $table_item(sort keys %table_trace_points) {
		my $used = 0;
		# Run through all specs
		foreach my $spec(sort keys %spec_trace_points) {
			# Run through all trace items in each spec
			foreach my $spec_item(sort keys %{$spec_trace_points{$spec}}) {
				# Set use flag if item is found
				if ($table_item eq $spec_item) {
					$used = 1;
					last;
				}
			}
			# Break spec loop; existing in at least one spec is sufficiently
			last if ($used == 1);
		}
		# If all specs are visited and item is still unusued ...
		if ($used == 0) {
			# ... make output, first one with heading
			if ($output == 0) {
				print "Items which are configured but not referenced by any spec:\n";
				$output = 1;
			}
			print "\t$table_item\n";
		}
	}
	# Separator from next block, only if there was output
	print "\n" if ($output == 1);
}

# Find items which are configured but not referenced by code
sub find_configured_items_not_referenced_by_code()
{
	my $output = 0;
	# Run through all configured trace items
	foreach my $item(sort keys %table_trace_points) {
		# Check if item is referenced by code
		if (not exists $code_trace_points{$item}) {
			# Not the case, make output, first one with heading
			if ($output == 0) {
				print "Items which are configured but not used in the code:\n";
				$output = 1;
			}
			print "\t$item\n";
		}
	}
	# Separator from next block, only if there was output
	print "\n" if ($output == 1);
}

# Find items of type X which are referenced by a spec as items of type Y
sub find_type_mismatches_by_spec()
{
	my $last_faulty_spec = "";
	
	# Run through all configured trace items
	foreach my $table_item(sort keys %table_trace_points) {
		# Store item type from config table
		my $type_config = $table_trace_points{$table_item}{Type};
		# Run through all specs
		foreach my $spec(sort keys %spec_trace_points) {
			# Run through all trace items in each spec
			foreach my $spec_item(sort keys %{$spec_trace_points{$spec}}) {
				# Store item type from current spec
				my $type_spec = $spec_trace_points{$spec}{$spec_item}{Type};
				# If items are the same, but type differs ...
				if ($table_item eq $spec_item and $type_config ne $type_spec) {
					# ... make output, with heading for first item
					if ($spec ne $last_faulty_spec) {
						# Separator if there was already output
						print "\n" if ($last_faulty_spec ne "");
						print "Items with conflicting types in \"$spec\":\n";
						$last_faulty_spec = $spec;
					}
					print "\t$table_item (config: $type_config, spec: $type_spec)\n";
				}
			}
		}
	}
	# Separator from next block, only if there was output
	print "\n" if ($last_faulty_spec ne "");
}

# Find items of type X which are referenced by the code as items of type Y
sub find_type_mismatches_by_code()
{
	my $output = 0;
	
	# Run through all configured items
	foreach my $item(sort keys %table_trace_points) {
		# Store type from configuration
		my $type_config = $table_trace_points{$item}{Type};
		# Check if that item also exists in the code
		if (exists $code_trace_points{$item}) {
			# Store type from code
			my $type_code = $code_trace_points{$item}{Type};
			# Check if types mismatch
			if ($type_config ne $type_code) {
				# Make output, with heading for first one
				if ($output == 0) {
					print "Items with conflicting types in code:\n";
					$output = 1;
				}
				print "\t$item (config: $type_config, code: $type_code)\n";
			}
		}
	}
	# Separator from next block, only if there was output
	print "\n" if ($output == 1);
}

# Find items which are configured but did not exist in old CSV file
sub find_new_items()
{
	my $output = 0;
	
	# Makes no sense if there is no old CSV file
	return if (not $has_old_csv);
	# Run through all configured items
	foreach my $item(sort keys %table_trace_points) {
		# Check if item did not exist in old CSV file
		if (not exists $old_csv_points{$item}) {
			# Make output: we found a new one
			if ($output == 0) {
				print "New items that did not exist in old CSV file:\n";
				$output = 1;
			}
			print "\t$item\n";
		}
	}
	# Separator from next block, only if there was output
	print "\n" if ($output == 1);
}

# Find items which exist in old CSV file but are not configured anymore
sub find_deleted_items()
{
	my $output = 0;
	
	# Makes no sense if there is no old CSV file
	return if (not $has_old_csv);
	# Run through all items from old CSV file
	foreach my $item(sort keys %old_csv_points) {
		# Check if item does not exist in configuration
		if (not exists $table_trace_points{$item}) {
			# Make output: we found a deleted item
			if ($output == 0) {
				print "Items that existed in old CSV file but have been deleted:\n";
				$output = 1;
			}
			print "\t$item\n";
		}
	}
	# Separator from next block, only if there was output
	print "\n" if ($output == 1);
}

# Perform validation tasks
sub validation()
{
	find_unknown_items_used_by_spec();
	find_configured_items_not_referenced_by_any_spec();
	find_configured_items_not_referenced_by_code();
	find_type_mismatches_by_spec();
	find_type_mismatches_by_code();
	find_new_items();
	find_deleted_items();
}

sub main()
{
	# Check if old CSV file exists
	$has_old_csv = 1 if (-e $old_csv_file);

	get_spec_trace_points($spec_dir);
	get_code_trace_points($pkg_dir);
	get_table_trace_points($cfg_table_file);
	read_csv_file($old_csv_file);
	write_csv_file($new_csv_file);
	validation();
}

main();
