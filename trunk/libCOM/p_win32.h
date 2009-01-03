/*
 * libcom: An implementation of the Component Object Model
 * @(#) $Id$
 */

/*
 * Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Mo McRoberts.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the author(s) of this software may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * AUTHORS OF THIS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef P_WIN32_H_
# define P_WIN32_H_                    1

/* This is tedious, but necessary. */
#  define GUID_NULL Win32_GUID_NULL

#  define IUnknown Win32_IUnknown
#  define IUnknownVtbl Win32_IUnknownVtbl
#  define IID_IUnknown Win32_IID_IUnknown

#  define IClassFactory Win32_IClassFactory
#  define IClassFactoryVtbl Win32_IClassFactoryVtbl
#  define IID_IClassFactory Win32_IID_IClassFactory

#  define IMalloc Win32_IMalloc
#  define IMallocVtbl Win32_IMallocVtbl
#  define IID_IMalloc Win32_IID_IMalloc

#  define IMallocSpy Win32_IMallocSpy
#  define IMallocSpyVtbl Win32_IMallocSpyVtbl
#  define IID_IMallocSpy Win32_IID_IMallocSpy

#  include <objbase.h>

#  undef GUID_NULL

#  undef IUnknown
#  undef IUnknownVtbl
#  undef IID_IUnknown

#  undef IClassFactory
#  undef IClassFactoryVtbl
#  undef IID_IClassFactory

#  undef IMalloc
#  undef IMallocVtbl
#  undef IID_IMalloc

#  undef IMallocSpy
#  undef IMallocSpyVtbl
#  undef IID_IMallocSpy

#endif /* !P_WIN32_H_ */
