#AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS], [TAR-NAME], [URL])
AC_INIT([simplang], [0.0.4], [foo@gmail.com], [simplang], [https://dylangentile.com])

# Force autoconf to be at least this version number.
AC_PREREQ([2.68])

# Put autotools auxiliary files in a subdir, so they don't clutter top dir.
# I am not actually doing this at the moment because there should be relatively
# little in the top directory anyway.
#AC_CONFIG_AUX_DIR([build-aux])
#AC_CONFIG_MACRO_DIR([m4])

# Enable "automake" to simplify creating makefiles. foreign relaxes some GNU
# checks. -Wall and -Werror are instructions to Automake, not gcc.
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# These are the files to be generated.
AC_CONFIG_FILES([Makefile src/Makefile])
#tests/Makefile

# Safety check - list a source file that wouldn't be in other directories.
AC_CONFIG_SRCDIR([src/main.cpp])

# Put configuration results in this file for us to #include in our program.
#AC_CONFIG_HEADERS([config.h])

# Check for a C++ compiler.
AC_PROG_CXX

DIALECT="-std=c++11"
echo 'int main() {return 0;}' > ./log.cpp && $CXX -std=c++11 ./log.cpp && echo "checking for c++11... yes" || AC_MSG_ERROR([c++ compiler does not support c++11])
rm ./log.cpp
rm a.out

# Check for libraries, headers, data etc here.

# Last line: actually generate all required output files.
AC_OUTPUT
