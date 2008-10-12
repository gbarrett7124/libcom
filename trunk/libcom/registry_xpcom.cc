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

#ifdef COM_USE_XPCOM

# include <nsXPCOM.h>
# include <nsIComponentRegistrar.h>
# include <nsIComponentManager.h>

static nsIComponentRegistrar *xpcom_registrar;
static nsIComponentManager *xpcom_components;

static void xpcom__convert_to_nsid(nsID &dest, com_rclsid_t src);

int
com__xpcom_registry_init(void)
{
	NS_GetComponentRegistrar(&xpcom_registrar);
	NS_GetComponentManager(&xpcom_components);
	return 0;
}

com_result_t
com__xpcom_register(com_rco_t *rcox, IClassFactory *factory)
{
	nsID classid;
	com_result_t r;
	
	r = COM_E_INVALIDARG;
	/* You can register a component by pathname if it's in-process */
	if(0 != (rcox->ctx & (COM_CTX_INPROC_SERVER|COM_CTX_INPROC_HANDLER)) &&
		NULL != rcox->modulepath)
	{

	}
	/* You can only register an existing factory if it's in a running server */
	if(COM_CTX_LOCAL_SERVER == (rcox->ctx & COM_CTX_LOCAL_SERVER) && NULL != factory)
	{
		xpcom__convert_to_nsid(classid, *(rcox->clsid));
		if(rcox->factory)
		{
			xpcom_registrar->RegisterFactory(classid, rcox->displayname, rcox->contractid, (nsIFactory *) factory);
			r = COM_S_OK;
		}
	}
	return r;
}

com_result_t
com__xpcom_unregister(com_rclsid_t rclsid, IClassFactory *factory)
{
	nsID clsid;
	
	xpcom__convert_to_nsid(clsid, rclsid);
	xpcom_registrar->UnregisterFactory(clsid, (nsIFactory *) factory);
	return COM_S_OK;
}

com_result_t
com__xpcom_getclass(com_rclsid_t rclsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out)
{
	nsID clsid, iid;
	
	(void) server;
	
	if(COM_CTX_INPROC_SERVER == (context & COM_CTX_INPROC_SERVER))
	{
		xpcom__convert_to_nsid(clsid, rclsid);
		xpcom__convert_to_nsid(iid, riid);
		xpcom_components->GetClassObject(clsid, iid, out);
		return COM_S_OK;
	}
	return COM_E_CLASSNOTREG;
}

static void
xpcom__convert_to_nsid(nsID &dest, com_rclsid_t src)
{
	dest.m0 = src.data1;
	dest.m1 = src.data2;
	dest.m2 = src.data3;
	memcpy(dest.m3, src.data4, sizeof(src.data4));
}

#endif /* COM_USE_XPCOM */
