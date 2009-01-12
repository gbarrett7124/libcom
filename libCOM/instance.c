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

com_result_t
RPC_SYM(com_register_factory)(com_rclsid_t clsid, com_context_t context, const char *contractid, IUnknown *factory, uint32_t *key)
{
	com_rco_t rcox;

	memset(&rcox, 0, sizeof(com_rco_t));
	rcox.clsid = clsid;
	rcox.ctx = context;
	rcox.contractid = contractid;
	rcox.factory = factory;
	return com_register(&rcox, key);
}

com_result_t
RPC_SYM(CoRegisterClassObject)(com_rclsid_t clsid, IUnknown *factory, com_context_t context, com_regflags_t flags, uint32_t *key)
{
	com_rco_t rcox;
	
	memset(&rcox, 0, sizeof(com_rco_t));
	rcox.clsid = clsid;
	rcox.factory = factory;
	rcox.ctx = context;
	rcox.flags = flags;
	return com_register(&rcox, key);
}

com_result_t
RPC_SYM(CoRevokeClassObject)(uint32_t key)
{
	return com_unregister(key);
}

com_result_t
RPC_SYM(CoGetClassObject)(com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out)
{
	return com_getclass(clsid, context, server, riid, out);
}

com_result_t
RPC_SYM(CoCreateInstanceEx)(com_rclsid_t clsid, IUnknown *outer, com_context_t context, com_server_t *server, size_t intfcount, com_multiqi_t *intf)
{
	IClassFactory *cf;
	IUnknown *inst;
	com_result_t r;
	size_t failed, c;
	
	/* Connect to the IClassFactory instance */
	if(COM_S_OK != (r = com_getclass(clsid, context, server, &IID_IClassFactory, (void **) &cf)))
		{
			return r;
		}
	/* Create an uninitialized instance of the class */
	r = IClassFactory_CreateInstance(cf, outer, &IID_IUnknown, (void **) &inst);
	IClassFactory_Release(cf);
	if(COM_S_OK != r)
		{
			return r;
		}
	/* Query the interface for the interfaces requested */
	failed = 0;
	for(c = 0; c < intfcount; c++)
		{
			intf[c].hr = IUnknown_QueryInterface(inst, intf[c].pIID, (void **) &(intf[c].pItf));
			if(intf[c].hr != COM_S_OK)
				{
					failed++;
				}
		}
	if(failed == intfcount)
		{
			return COM_E_NOINTERFACE;
		}
	if(failed)
		{
			return COM_S_NOTALLINTERFACES;
		}
	return COM_S_OK;
}

com_result_t
RPC_SYM(CoCreateInstance)(com_rclsid_t clsid, IUnknown *outer, unsigned context, com_riid_t riid, void **out)
{
	IClassFactory *cf;
	com_result_t r;
	
	/* Connect to the IClassFactory instance */
	if(COM_S_OK != (r = com_getclass(clsid, context, NULL, &IID_IClassFactory, (void **) &cf)))
		{
			return r;
		}
	/* Create an uninitialized instance of the class */
	r = IClassFactory_CreateInstance(cf, outer, riid, out);
	IClassFactory_Release(cf);
	return r;
}
