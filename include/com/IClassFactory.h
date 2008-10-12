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

#ifndef COM_ICLASSFACTORY_H_
# define COM_ICLASSFACTORY_H_          1

# include "com/com.h"

# include "com/IUnknown.h"

# undef INTERFACE
# define INTERFACE                     IClassFactory

DECLARE_INTERFACE_(IClassFactory, IUnknown)
{
	BEGIN_INTERFACE
	
	STDMETHOD(QueryInterface)(THIS_ com_riid_t riid, void **out) PURE;
	STDMETHOD_(unsigned long,AddRef)(THIS) PURE;
	STDMETHOD_(unsigned long,Release)(THIS) PURE;
	
	STDMETHOD(CreateInstance)(THIS_ IUnknown *outer, com_riid_t riid, void **out) PURE;
	STDMETHOD(LockServer)(THIS_ com_bool_t lock) PURE;
	
	END_INTERFACE
};

# if !defined(COM_CINTERFACE) && defined(__cplusplus)
#  define IClassFactory_QueryInterface(__this, riid, out) __this->QueryInterface(riid, out)
#  define IClassFactory_AddRef(__this) __this->AddRef()
#  define IClassFactory_Release(__this) __this->Release()
#  define IClassFactory_CreateInstance(__this, outer, riid, out) __this->CreateInstance(outer, riid, out)
#  define IClassFactory_LockServer(__this, lock) __this->LockServer(lock)
# else
#  define IClassFactory_QueryInterface(__this, riid, out) __this->lpVtbl->QueryInterface(__this, riid, out)
#  define IClassFactory_AddRef(__this) __this->lpVtbl->AddRef(__this)
#  define IClassFactory_Release(__this) __this->lpVtbl->Release(__this)
#  define IClassFactory_CreateInstance(__this, outer, riid, out) __this->lpVtbl->CreateInstance(__this, outer, riid, out)
#  define IClassFactory_LockServer(__this, lock) __this->lpVtbl->LockServer(__this, lock)
# endif

#endif /* !COM_ICLASSFACTORY_H_ */

DEFINE_GUID(IID_IClassFactory, 0x00000001, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);
