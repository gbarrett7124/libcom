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

#if defined(HAVE_WINDOWS_H)

#include <stdio.h>
#include <windows.h>
#include "com/com.h"

/* Called by COM to request the module register its classes. We determine
 * the filename of the module and pass it on to com_self_register().
 */
COM_EXPORTED com_result_t __stdcall
DllRegisterServer(void)
{
	HRESULT hr;
	HMODULE self;
	char filename[1024];
	
	if(0 == GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR) (const void *) com_self_register, &self))
	{
		return (com_result_t) GetLastError();
	}
	GetModuleFilenameA(self, filename, sizeof(filename));
	filename[sizeof(filename) - 1] = 0;
	return com_self_register(filename);
}

COM_EXPORTED com_result_t __stdcall
DllUnregisterServer(void)
{
	fprintf(stderr, "[DllUnregisterServer]\n");
	return com_self_unregister();
}

COM_EXPORTED com_result_t __stdcall
DllCanUnloadNow(void)
{
	if(0 == com_self_lockcount())
	{
		return COM_S_OK;
	}
	return COM_S_FALSE;
}

COM_EXPORTED com_result_t __stdcall
DllGetClassObject(com_rclsid_t rclsid, com_riid_t riid, void **out)
{
	fprintf(stderr, "[DllGetClassObject]\n");
	return com_self_getclass(rclsid, riid, out);
}

#endif /* HAVE_WINDOWS_H */

