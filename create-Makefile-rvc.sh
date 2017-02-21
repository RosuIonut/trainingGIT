#!/bin/bash

# Create the file "Makefile-rvc".
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
cd ../libUtils
$PWD/Make-rvc.sh
cd ../navi_drawer

echo "------ Creating Makefile (RVC) ------"

# CROSS_COMPILE and CROSS_ROOTFS are used by the mkspecs.
export ROOTFS_RVC=/opt/rvc/roofs/rootfs
export CROSS_COMPILE=/opt/rvc/usr/local/gcc-4.3.3-glibc-2.9-nptl/i586-pc-linux-gnu/bin/i586-pc-linux-gnu-
export CROSS_ROOTFS=${ROOTFS_RVC}

# Don't change paths that are already correctly based on the rootfs.
# Note that because of symbolic links, the part of the path before
# "rootfs" can vary.
FIND_1=rootfs/usr/local/Qt
REPL_1=rootfs/usr/local/Qt
# Escape "/"
FIND_1=${FIND_1//\//\\\/}
REPL_1=${REPL_1//\//\\\/}

# Fix the rootfs.
FIND_2=/usr/local/Qt
REPL_2=${ROOTFS_RVC}/usr/local/Qt
# Escape "/"
FIND_2=${FIND_2//\//\\\/}
REPL_2=${REPL_2//\//\\\/}

# Expand the variable DEV_IMAGE (embedded by the mkspecs) with the rootfs.
FIND_5='${DEV_IMAGE}'
REPL_5=${ROOTFS_RVC}
# Escape "/"
REPL_5=${REPL_5//\//\\\/}

# As a safety measure, the makefile is prevented from inadvertently using 
# an incorrectly generated makefile, by commenting out the qmake comand 
# with the command false.
# The false command will cause an error to be detected instead of executing 
# qmake.
FIND_3="QMAKE         ="
REPL_3="QMAKE         = false"

QTPATH=${ROOTFS_RVC}/usr/local/Qt 

${QTPATH}/bin/qmake -o - "PLATFORM=rvc" navi_drawer.pro -spec ${QTPATH}/mkspecs/linux-g++-rvc | \
sed \
--expression="s/${FIND_1}/XX_1_XX/g" \
--expression="s/${FIND_2}/XX_2_XX/g" \
--expression="s/${FIND_5}/XX_5_XX/g" \
--expression="s/${FIND_3}/XX_3_XX/g" \
--expression="s/XX_1_XX/${REPL_1}/g" \
--expression="s/XX_2_XX/${REPL_2}/g" \
--expression="s/XX_5_XX/${REPL_5}/g" \
--expression="s/XX_3_XX/${REPL_3}/g" \
> Makefile

echo "------ Created Makefile (RVC) ------"
