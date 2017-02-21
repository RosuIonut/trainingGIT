#!/bin/bash

# Create the file "Makefile-pc".
#
# The problem is that qmake has hard coded directory paths, and these do 
# not match our installation.
#
# This script works by using qmake to create the Makefile, and pipes it 
# through a sed script to replace incorrect strings with correct strings.
#
# To simplify the writing of the sed script the search strings are defined
# using variables.  Below is a list of strings to find and their replacements,
# named FIND_n and REPL_n respectively.
#
# Because a find string can be a substring of a replacement string, the 
# replacement is done in 2 phases to avoid recursive substitution.
# First all the FIND_n strings are replaced with placeholders,
# and then all the place holders are replaced with the REPL_n strings.
# This implies also that the order of FIND_n strings is important. 
#
# A complication is that sed delimits the search strings with '/',
# and this character appears very often in directory paths!
# To include '/' in the search strings it must be escaped by '\'.
# For this we get some help from the shell.  It will expand:
#   "${VAR//\//\\\/}"
# to the value ${VAR} with each '/' replaced by '\/'.

# Build libutils
cd ../libUtils/
$PWD/Make-pc.sh
cd ../navi_drawer

echo "------ Creating Makefile (PC) ------"

QTPATH=/usr/share/qt4/

qmake -o - "PLATFORM=pc" navi_drawer.pro -spec ${QTPATH}/mkspecs/linux-g++ > Makefile

echo "------ Created Makefile (PC) ------"
