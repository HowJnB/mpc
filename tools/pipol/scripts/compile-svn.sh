#!/bin/sh
if [[ $# -lt 1 ]]; then
    echo Usage: `basename $0` dir [options];
    echo -n "Configure, compile, and check sources in \$PIPOL_WDIR/dir "
    echo passing options to the compiler;
    exit 1;
else
    echo \*
    echo \* $0 $*
    echo \*
fi

cd $PIPOL_WDIR/$1
if [[ ! -f configure ]]; then
	autoreconf -i || exit 1
fi

shift
if grep -q "CFLAGS=" <<<"$@"; then
# "$@" would also split the CFLAGS options so we have to separate configure
# and CFLAGS options manually
    CONFIGURE_OPTIONS=`sed -ne 's/\(.*\)\ *CFLAGS=\"\(.*\)\"\(.*\)/\1\3/p' <<<"$@"`
    CFLAGS_OPTIONS=`sed -ne 's/\(.*\)\ *CFLAGS=\"\(.*\)\"\(.*\)/CFLAGS=\2/p' <<<"$@"`
    ./configure $CONFIGURE_OPTIONS "$CFLAGS_OPTIONS" 
else
    ./configure "$@"
fi
if [ $? != 0 ]; then
# configure failed
    cat config.log
    exit 1
fi
make && make check
