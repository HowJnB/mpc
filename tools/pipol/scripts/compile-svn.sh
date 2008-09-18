#!/bin/sh
if [ $# -lt 1 ]; then
	echo Usage: `basename $0` dir [options];
	echo -n "Configure, compile, and check sources in \$PIPOL_WDIR/dir "
	echo passing options to the compiler;
	exit 1;
	
	else
	echo \*; echo \* $0 $*; echo \*;
fi

cd $PIPOL_WDIR/$1
shift
autoreconf -i && ./configure $* && make && make check;
