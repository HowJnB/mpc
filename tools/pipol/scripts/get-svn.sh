#!/bin/sh
#
# Install sources from subversion repository in /pipol
#
REPO="https://scm.gforge.inria.fr/svn/"
if [ $# -ne 2 ]; then
	echo "Usage: $0 svn_branch install_dir"
	echo -n "Check out code source from $REPO/svn_branch "
	echo "into \$PIPOL_WDIR/install_dir"
	exit 1;
fi

svn co ${REPO}$1 ${PIPOL_WDIR}/$2

#cd $PIPOL_WDIR
#svn co svn://scm.gforge.inria.fr/svn/mpfr/trunk svn-mpfr
#svn co https://scm.gforge.inria.fr/svn/mpc/trunk svn-mpc
