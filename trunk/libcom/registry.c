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

#ifdef COM_USE_LOCALREG
typedef struct registry_entry_struct registry_entry_t;

struct registry_entry_struct
{
	com_clsid_t clsid;
	IUnknown *factory;
	com_context_t ctx;
	com_regflags_t flags;
	char *modulepath;
	void *module;
	com_getclassobject_t gco;
};

static uint32_t entrykey = 1;
static registry_entry_t *entries;
static size_t nentries;
#endif

com_result_t
COM_SYM(com_register)(com_rco_t *rcox, uint32_t *key)
{
	com_result_t r = COM_E_ACCESSDENIED;
	
#ifdef COM_USE_LOCALREG

#endif
#ifdef COM_USE_XPCOM
	if(COM_S_OK != (r = xpcom_register(rcox, key)))
	{
		return r;
	}
#endif
	return r;
}

com_result_t
COM_SYM(com_unregister)(uint32_t key)
{
	return COM_S_OK;
}

com_result_t
COM_SYM(com_getclass)(com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out)
{
#ifdef COM_USE_XPCOM
	return xpcom_getclass(clsid, context, server, riid, out);
#endif
	return COM_E_ACCESSDENIED;
}
