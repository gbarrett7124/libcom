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

#ifndef COM_IMALLOC_H_
# define COM_IMALLOC_H_                1

# include "com/com.h"

# include "com/IUnknown.h"

# undef INTERFACE
# define INTERFACE                     IMalloc

DECLARE_INTERFACE_(IMalloc, IUnknown)
{
	BEGIN_INTERFACE
	
	STDMETHOD(QueryInterface)(THIS_ com_riid_t riid, void **out) PURE;
	STDMETHOD_(uint32_t,AddRef)(THIS) PURE;
	STDMETHOD_(uint32_t,Release)(THIS) PURE;
	
	STDMETHOD_(void *,Alloc)(THIS_ com_size_t cb) PURE;
	STDMETHOD_(void *,Realloc)(THIS_ void *mem, com_size_t cb) PURE;
	STDMETHOD_(void,Free)(THIS_ void *mem) PURE;
	STDMETHOD_(com_size_t,GetSize)(THIS_ void *mem) PURE;
	STDMETHOD_(int,DidAlloc)(THIS_ void *mem) PURE;
	STDMETHOD_(void,HeapMinimize)(THIS) PURE;
	
	END_INTERFACE
};

# if defined(__cplusplus)
#  define IMalloc_QueryInterface(__this, riid, out) __this->QueryInterface(riid, out)
#  define IMalloc_AddRef(__this) __this->AddRef()
#  define IMalloc_Release(__this) __this->Release()
#  define IMalloc_Alloc(__this, cb) __this->Alloc(cb)
#  define IMalloc_Realloc(__this, mem, cb) __this->Realloc(mem, cb)
#  define IMalloc_Free(__this, mem) __this->Free(mem)
#  define IMalloc_GetSize(__this, mem) __this->GetSize(mem)
#  define IMalloc_DidAlloc(__this, mem) __this->DidAlloc(mem)
#  define IMalloc_HeapMinimize(__this) __this->HeapMinimize()
# else
#  define IMalloc_QueryInterface(__this, riid, out) __this->lpVtbl->QueryInterface(__this, riid, out)
#  define IMalloc_AddRef(__this) __this->lpVtbl->AddRef(__this)
#  define IMalloc_Release(__this) __this->lpVtbl->Release(__this)
#  define IMalloc_Alloc(__this, cb) __this->lpVtbl->Alloc(__this, cb)
#  define IMalloc_Realloc(__this, mem, cb) __this->lpVtbl->Realloc(__this, mem, cb)
#  define IMalloc_Free(__this, mem) __this->lpVtbl->Free(__this, mem)
#  define IMalloc_GetSize(__this, mem) __this->lpVtbl->GetSize(__this, mem)
#  define IMalloc_DidAlloc(__this, mem) __this->lpVtbl->DidAlloc(__this, mem)
#  define IMalloc_HeapMinimize(__this) __this->lpVtbl->HeapMinimize(__this)
# endif

#endif /* !COM_IMALLOC_H_ */

DEFINE_GUID(IID_IMalloc, 0x00000002, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);
DEFINE_GUID(IID_IMallocSpy, 0x0000001d, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);
