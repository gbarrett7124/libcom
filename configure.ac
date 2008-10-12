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

CPPFLAGS="$CPPFLAGS -W -Wall -D_REENTRANT"
export CPPFLAGS

CFLAGS="$CFLAGS -g -O0"
export CFLAGS

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

AC_ARG_ENABLE(xpcom, [AS_HELP_STRING([--enable-xpcom],[Enable the libcom/XPCOM bridge (default=auto)])],[use_xpcom=$enableval],[use_xpcom=auto])
AC_ARG_ENABLE(win32, [AS_HELP_STRING([--enable-win32],[Enable the libcom/Win32 bridge (default=auto)])],[use_win32=$enableval],[use_win32=auto])
AC_ARG_ENABLE(cf, [AS_HELP_STRING([--enable-cf],[Use CoreFoundation’s CFPreferences to store registration data  (default=auto)])],[use_cf=$enableval],[use_cf=auto])
AC_ARG_ENABLE(pthread, [AS_HELP_STRING([--enable-pthread],[Use POSIX threads (default=auto)])],[use_pthread=$enableval],[use_pthread=auto])
AC_ARG_ENABLE(dl, [AS_HELP_STRING([--enable-dl],[Use dlopen() for dynamic loading (default=auto)])],[use_dl=$enableval],[use_dl=auto])

AC_MSG_CHECKING([whether to use POSIX threads])
AC_MSG_RESULT([${use_pthread}])
if test x"${use_pthread}" = x"auto" || test x"${use_pthread}" = x"yes" ; then
	pthread_present=yes
	AC_CHECK_HEADER([pthread.h],[AC_DEFINE_UNQUOTED([HAVE_PTHREAD_H],1,[Define to 1 if you have the <pthread.h> header file.])],pthread_present=no)
	if test x"${pthread_present}" = x"yes" ; then
		AC_CHECK_LIB(pthread,pthread_create)
		AC_CHECK_FUNC(pthread_create,,pthread_present=no)
	fi
	if test x"${pthread_present}" = x"no" && test x"${use_pthread}" = x"yes"; then
		AC_MSG_ERROR([POSIX threads support was requested but it cannot be located])
	fi
	use_pthread=${pthread_present}
fi

AC_MSG_CHECKING([whether to use dlopen()])
AC_MSG_RESULT([${use_dl}])
if test x"${use_dl}" = x"auto" || test x"${use_dl}" = x"yes" ; then
	dl_present=yes
	AC_CHECK_HEADER([dlfcn.h],,dl_present=no)
	if test x"${dl_present}" = x"yes" ; then
		AC_CHECK_LIB(dl,dlopen)
		AC_CHECK_FUNC(dlopen,,dl_present=no)
	fi
	if test x"${dl_present}" = x"yes" && test x"${use_dl}" = x"yes" ; then
		AC_MSG_ERROR([dlopen() support was requested but it cannot be located])
	fi
	use_dl=${dl_present}
fi

AC_MSG_CHECKING([whether to use XPCOM])
AC_MSG_RESULT([${use_xpcom}])
if test x"${use_xpcom}" = x"auto" || test x"${use_xpcom}" = x"yes" ; then
	AC_PATH_PROG([XULRUNNER_CONFIG], [xulrunner-config],,)
	if ! test x"${XULRUNNER_CONFIG}" = x"" ; then
		AC_MSG_CHECKING([for XPCOM])
		XPCOM_LIBS=`${XULRUNNER_CONFIG} --libs xpcom`
		if test x"${XPCOM_LIBS}" = x"" ; then
			AC_MSG_RESULT([not available])
			if test x"${use_xpcom}" = x"yes" ; then
				AC_MSG_ERROR([--enable-xpcom was specified but XPCOM cannot be found])
			fi
			use_xpcom=no
			XPCOM_CPPFLAGS=''
		else
			use_xpcom=yes
			XPCOM_CPPFLAGS=`${XULRUNNER_CONFIG} --cflags xpcom`
			AC_MSG_RESULT([yes, libraries: ${XPCOM_LIBS}; headers: ${XPCOM_CPPFLAGS}])
		fi
	elif test x"${use_xpcom}" = x"yes" ; then
		AC_MSG_ERROR([xulrunner-config is required to configure the XPCOM bridge but it cannot be found])
	fi
fi

if test x"${use_xpcom}" = x"yes" ; then
	AC_DEFINE_UNQUOTED([COM_USE_XPCOM], 1, [Define to use XPCOM bindings])
else
	use_xpcom=no
fi
if test x"${use_win32}" = x"yes" ; then
	AC_DEFINE_UNQUOTED([COM_USE_WIN32], 1, [Define to use Win32 bindings])
else
	use_win32=no
fi
if test x"${use_cf}" = x"yes" ; then
	AC_DEFINE_UNQUOTED([COM_USE_CF], 1, [Define to use CoreFoundation for registration data])
else
	use_cf=no
fi
if test x"${use_pthread}" = x"yes" ; then
	AC_DEFINE_UNQUOTED([COM_USE_PTHREAD], 1, [Define to use POSIX threads for locking])
else
	use_pthread=no
fi
if test x"${use_dl}" = x"yes" ; then
	AC_DEFINE_UNQUOTED([COM_USE_DL], 1, [Define to use dlopen() for dynamic loading])
else
	use_dl=no
fi

AC_SUBST([XPCOM_LIBS])
AC_SUBST([XPCOM_CPPFLAGS])

AC_CONFIG_HEADER([config.h])

AC_CONFIG_FILES([Makefile
include/Makefile
include/com/Makefile
libcom/Makefile
tests/Makefile
])

AC_OUTPUT

AC_MSG_RESULT([
libcom configuration summary:
	XPCOM bridge:            ${use_xpcom}
	Win32 bridge:            ${use_win32}
	CoreFoundation support:  ${use_cf}
	POSIX threads:           ${use_pthread}
	dlopen() support:        ${use_dl}
])
