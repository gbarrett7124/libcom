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

#ifndef COM_ICOREGISTRY_H_
# define COM_ICOREGISTRY_H_            1

# include "COM/COM.h"

# undef INTERFACE
# define INTERFACE                     ICoRegistry

DECLARE_INTERFACE_(ICoRegistry, IUnknown)
{
	BEGIN_INTERFACE
	
	STDMETHOD(QueryInterface)(THIS_ com_riid_t riid, void **out) PURE;
	STDMETHOD_(uint32_t,AddRef)(THIS) PURE;
	STDMETHOD_(uint32_t,Release)(THIS) PURE;
	
	STDMETHOD(RegisterClass)(THIS_ com_rco_t *rco, uint32_t *key);
	STDMETHOD(UnregisterClassKey)(THIS_ uint32_t key) PURE;
	STDMETHOD(UnregisterClassID)(THIS_ com_rclsid_t rclsid) PURE;
	STDMETHOD(GetClassObject)(THIS_ com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out) PURE;
	END_INTERFACE
};

# if defined(COM_CINTERFACE) || !defined(__cplusplus)
#  define ICoRegistry_QueryInterface(__this, riid, out) __this->lpVtbl->QueryInterface(__this, riid, out)
#  define ICoRegistry_AddRef(__this) __this->lpVtbl->AddRef(__this)
#  define ICoRegistry_Release(__this) __this->lpVtbl->Release(__this)
#  define ICoRegistry_RegisterClass(__this, rco, key) __this->lpVtbl->RegisterClass(__this, rco, key)
#  define ICoRegistry_UnregisterClassKey(__this, key) __this->lpVtbl->UnregisterClassKey(__this, key)
#  define ICoRegistry_UnregisterClassID(__this, rcslid) __this->lpVtbl->UnregisterClassID(__this, rcslid)
#  define ICoRegistry_GetClassObject(__this, rclsid, ctx, server, riid, out) __this->lpVtbl->GetClassObject(__this, rclsid, ctx, server, riid, out)
# endif

#endif /* !COM_ICoRegistry_H_ */

DEFINE_GUID(IID_ICoRegistry, 0x5AFCD459, 0x00FE, 0x47B9, 0xB3, 0xD6, 0xF7, 0xDE, 0xA5, 0xD8, 0x90, 0xAC);
