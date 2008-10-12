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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "p_libcom.h"

#if defined(COM_USE_WIN32) && !defined(COM_USE_XPCOM)

IMalloc *
COM_SYM(com_allocator)(void)
{
	union {
		Win32_IMalloc p;
		IMalloc m;
	} *d;
	
	if(COM_S_OK != CoGetMalloc(1, (Win32_IMalloc **) &d))
	{
		return NULL;
	}
	return &(d->m);
}

com_result_t
COM_COMPAT(CoGetMalloc)(uint32_t context, IMalloc **out)
{
	HRESULT r;
	union {
		Win32_IMalloc p;
		IMalloc m;
	} *d;
	
	if(1 != context)
	{
		return COM_E_INVALIDARG;
	}
	if(COM_S_OK != (r = CoGetMalloc(1, (Win32_IMalloc **) &d)))
	{
		return (com_result_t) r;
	}
	*out = &(d->m);
	return COM_S_OK;
}

void *
COM_COMPAT(CoTaskMemAlloc)(com_size_t size)
{
	return CoTaskMemAlloc(size);
}

void
COM_COMPAT(CoTaskMemFree)(void *ptr)
{
	return CoTaskMemFree(ptr);
}

void *
COM_COMPAT(CoTaskMemRealloc)(void *ptr, com_size_t newsize)
{
	return CoTaskMemRealloc(ptr, newsize);
}

#endif /* COM_USE_WIN32 && !COM_USE_XPCOM */
