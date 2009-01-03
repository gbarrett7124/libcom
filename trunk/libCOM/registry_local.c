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

static com_result_t __stdcall CoLocalReg_QueryInterface(ICoRegistry *intf, com_riid_t iid, void **out);
static uint32_t __stdcall CoLocalReg_AddRef(ICoRegistry *intf);
static uint32_t __stdcall CoLocalReg_Release(ICoRegistry *intf);
static com_result_t __stdcall CoLocalReg_RegisterClass(ICoRegistry *intf, com_rco_t *rco, uint32_t *key);
static com_result_t __stdcall CoLocalReg_UnregisterClassKey(ICoRegistry *intf, uint32_t key);
static com_result_t __stdcall CoLocalReg_UnregisterClassID(ICoRegistry *intf, com_rclsid_t clsid);
static com_result_t __stdcall CoLocalReg_GetClassObject(ICoRegistry *intf, com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out);

# ifdef COM_USE_PTHREAD
static pthread_mutex_t reglock = PTHREAD_MUTEX_INITIALIZER;
#  define REGLOCK pthread_mutex_lock(&reglock)
#  define REGUNLOCK pthread_mutex_unlock(&reglock)
# else
#  define REGLOCK
#  define REGUNLOCK
# endif

typedef struct localreg_entry_struct localreg_entry_t;

struct localreg_entry_struct
{
	com_rco_t rco;
	uint32_t key;
	uint32_t refcount;
};

static uint32_t entrykey = 1;
static localreg_entry_t *entries;
static size_t nentries;

DECLARE_CLASS(CoLocalReg)
{
	DECLARE_IMPLEMENTS(CoLocalReg, ICoRegistry);
};

DEFINE_CLASS_INTERFACE(CoLocalReg, ICoRegistry)
{
	CoLocalReg_QueryInterface,
	CoLocalReg_AddRef,
	CoLocalReg_Release,
	CoLocalReg_RegisterClass,
	CoLocalReg_UnregisterClassKey,
	CoLocalReg_UnregisterClassID,
	CoLocalReg_GetClassObject
};

DEFINE_STATIC_CLASS(CoLocalReg, com__registry_local)
{
	DEFINE_STATIC_IMPLEMENTATION(CoLocalReg, ICoRegistry)
};

com_result_t
com__registry_local_init(void)
{
	ICoRegistry *reg;
	com_result_t r;
	
	INITIALISE_ADDREF_INTERFACE_POINTER(CoLocalReg, &com__registry_local, ICoRegistry);
	reg = GET_INTERFACE_POINTER(&com__registry_local, ICoRegistry);
	r = com_registry_add(reg);
	ICoRegistry_Release(reg);
	return r;
}

static com_result_t __stdcall
CoLocalReg_QueryInterface(ICoRegistry *intf, com_riid_t iid, void **out)
{
	if(com_guid_equal(iid, &IID_IUnknown) || com_guid_equal(iid, &IID_ICoRegistry))
	{
		ICoRegistry_AddRef(intf);
		*out = intf;
		return COM_S_OK;
	}
	return COM_E_NOINTERFACE;
}

static uint32_t __stdcall
CoLocalReg_AddRef(ICoRegistry *intf)
{
	(void) intf;
	
	return 2;
}

static uint32_t __stdcall
CoLocalReg_Release(ICoRegistry *intf)
{
	(void) intf;
	
	return 1;
}

static com_result_t __stdcall
CoLocalReg_RegisterClass(ICoRegistry *intf, com_rco_t *rco, uint32_t *key)
{
	localreg_entry_t *q;
	size_t c, freeslot;

	(void) intf;
	
	if(NULL == rco->factory)
	{
		/* For now, we only support registrations which include the factory
		 * instance.
		 */
		return COM_E_NOT_SUPPORTED;
	}
	REGLOCK;
	freeslot = 0;
	for(c = 0; c < nentries; c++)
	{
		if(0 == entries[c].refcount)
		{
			freeslot = c + 1;
		}
		else if(0 != entries[c].refcount && com_guid_equal(rco->clsid, entries[c].rco.clsid))
		{
			entries[c].refcount++;
			*key = entries[c].key;
			REGUNLOCK;
			return COM_S_OK;
		}
	}
	if(0 != freeslot)
	{
		q = &(entries[freeslot - 1]);
	}
	else
	{
		if(NULL == (q = (localreg_entry_t *) realloc(entries, sizeof(localreg_entry_t) * (nentries + 1))))
		{
			REGUNLOCK;
			return COM_E_OUTOFMEMORY;
		}
		entries = q;
		q = &(entries[nentries]);
	}
	memset(q, 0, sizeof(localreg_entry_t));
	IUnknown_AddRef(rco->factory);
	q->refcount = 1;
	memcpy(&(q->rco), rco, sizeof(com_rco_t));
	*key = q->key = entrykey;
	nentries++;
	entrykey++;
	REGUNLOCK;
	return COM_S_OK;
}

static com_result_t __stdcall
CoLocalReg_UnregisterClassKey(ICoRegistry *intf, uint32_t key)
{
	size_t c;
	
	(void) intf;
	
	REGLOCK;
	for(c = 0; c < nentries; c++)
	{
		if(0 != entries[c].refcount && entries[c].key == key)
		{
			entries[c].refcount--;
			if(0 == entries[c].refcount)
			{
				IUnknown_Release(entries[c].rco.factory);
				memset(&(entries[c]), 0, sizeof(localreg_entry_t));
			}
			REGUNLOCK;
			return COM_S_OK;
		}
	}
	REGUNLOCK;
	return COM_E_CLASSNOTREG;
}

static com_result_t __stdcall
CoLocalReg_UnregisterClassID(ICoRegistry *intf, com_rclsid_t clsid)
{
	size_t c;
	
	(void) intf;
	
	REGLOCK;
	for(c = 0; c < nentries; c++)
	{
		if(0 != entries[c].refcount && com_guid_equal(entries[c].rco.clsid, clsid))
		{
			entries[c].refcount--;
			if(0 == entries[c].refcount)
			{
				IUnknown_Release(entries[c].rco.factory);
				memset(&(entries[c]), 0, sizeof(localreg_entry_t));
			}
			REGUNLOCK;
			return COM_S_OK;
		}
	}
	REGUNLOCK;
	return COM_E_CLASSNOTREG;
}

static com_result_t __stdcall
CoLocalReg_GetClassObject(ICoRegistry *intf, com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out)
{
	size_t c;
	localreg_entry_t *p;
	com_result_t r;
	
	(void) intf;
	
	p = NULL;
	REGLOCK;
	for(c = 0; c < nentries; c++)
	{
		if(0 != entries[c].refcount && com_guid_equal(entries[c].rco.clsid, clsid))
		{
			p = &(entries[c]);
			p->refcount++;
			REGUNLOCK;
			break;
		}
	}
	REGUNLOCK;
	if(NULL == p)
	{
		return COM_E_CLASSNOTREG;
	}
	if(NULL != p->rco.factory)
	{
		r = IUnknown_QueryInterface(p->rco.factory, riid, out);
		p->refcount--;
		return r;
	}
	return COM_E_CLASSNOTREG;
}

#endif /* !COM_USE_LOCALREG */