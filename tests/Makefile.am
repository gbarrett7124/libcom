## @(#) $Id$

## Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Mo McRoberts.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
## 1. Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
## 3. The names of the author(s) of this software may not be used to endorse
##    or promote products derived from this software without specific prior
##    written permission.
##
## THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
## INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
## AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
## AUTHORS OF THIS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
## SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
## TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
## PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
## LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
## NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
## SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CPPFLAGS += -I${top_srcdir}/include -I${top_builddir}/include

noinst_LTLIBRARIES = test-3-server.la

noinst_PROGRAMS = test-1 test-2

test_1_SOURCES = test-1.c
test_1_LDADD = ../libCOM/libCOM.la

test_2_SOURCES = test-2-server.c test-2-factory.c test-2.c test-2-guids.h ITester2.h Test2.h Test2Factory.h
test_2_LDADD = ../libCOM/libCOM.la

test_3_server_la_LDFLAGS = -module -avoid-version -export-dynamic -shared -rpath `pwd` -no-install
test_3_server_la_SOURCES = test-3-server.c test-3-factory.c
test_3_server_la_LIBADD = ../libCOM-stubs/libCOM-stubs.la
