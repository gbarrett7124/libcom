dnl libcom: An implementation of the Component Object Model
dnl @(#) $Id$

dnl Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Mo McRoberts.
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions
dnl are met:
dnl 1. Redistributions of source code must retain the above copyright
dnl    notice, this list of conditions and the following disclaimer.
dnl 2. Redistributions in binary form must reproduce the above copyright
dnl    notice, this list of conditions and the following disclaimer in the
dnl    documentation and/or other materials provided with the distribution.
dnl 3. The names of the author(s) of this software may not be used to endorse
dnl    or promote products derived from this software without specific prior
dnl    written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
dnl INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
dnl AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
dnl AUTHORS OF THIS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
dnl TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
dnl PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
dnl LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
dnl NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
dnl SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

AC_INIT(libcom, 0.0)
AC_CONFIG_SRCDIR([include/com/com.h])
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

CPPFLAGS="$CPPFLAGS -W -Wall"
export CPPFLAGS

CFLAGS="$CFLAGS -g -O0"
export CFLAGS

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

AC_PATH_PROG([XULRUNNER_CONFIG], [xulrunner-config],,)

if ! test x"${XULRUNNER_CONFIG}" = x"" ; then
	AC_MSG_CHECKING([for XPCOM])
	XPCOM_LIBS=`${XULRUNNER_CONFIG} --libs xpcom`
	if test x"${XPCOM_LIBS}" = x"" ; then
		AC_MSG_RESULT([not available])
		use_xpcom=no
		XPCOM_CPPFLAGS=''
	else
		use_xpcom=yes
		XPCOM_CPPFLAGS=`${XULRUNNER_CONFIG} --cflags xpcom`
		AC_MSG_RESULT([yes, libraries: ${XPCOM_LIBS}; headers: ${XPCOM_CPPFLAGS}])
	fi
fi

if test x"${use_xpcom}" = x"yes" ; then
	AC_DEFINE_UNQUOTED([COM_USE_XPCOM], 1, [Define to use XPCOM bindings])
fi

AC_SUBST([XPCOM_LIBS])
AC_SUBST([XPCOM_CPPFLAGS])

AC_CONFIG_HEADER([config.h])

AC_CONFIG_FILES([Makefile
include/Makefile
include/com/Makefile
libcom/Makefile
])

AC_OUTPUT
