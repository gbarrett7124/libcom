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

#define COM_CTX_LOCALREG \
	(COM_CTX_LOCAL_SERVER|COM_CTX_INPROC_SERVER|COM_CTX_INPROC_HANDLER)

typedef struct registry_entry_struct registry_entry_t;

struct registry_entry_struct
{
	com_clsid_t clsid;
	IUnknown *factory;
	IClassFactory *cf;
	com_context_t ctx;
	com_regflags_t flags;
	char *modulepath;
	void *module;
	com_getclassobject_t gco;
	uint32_t key;
#ifdef COM_USE_WIN32
	uint32_t rkey;
#endif
};

static uint32_t entrykey = 1;
static registry_entry_t *entries;
static size_t nentries;
#ifdef COM_USE_PTHREAD
static pthread_mutex_t registrylock = PTHREAD_MUTEX_INITIALIZER;
#endif

static void com__registry_lock(void);
static void com__registry_unlock(void);
static com_result_t com__unregister_entry(size_t index);

/* FIXME: Support for COM_REG_SUSPENDED */
com_result_t
COM_SYM(com_register)(com_rco_t *rcox, uint32_t *key)
{
	com_result_t r = COM_E_ACCESSDENIED;
	registry_entry_t *q;
	
	com__registry_lock();
	if(NULL == (q = realloc(entries, sizeof(registry_entry_t) * (nentries + 1))))
	{
		com__registry_unlock();
		return COM_E_OUTOFMEMORY;
	}
	entries = q;
	q = &(entries[nentries]);
	memset(q, 0, sizeof(registry_entry_t));
	q->clsid = *(rcox->clsid);
	q->key = entrykey;
	q->ctx = rcox->ctx;
	q->flags = rcox->flags;
	if(NULL != rcox->factory)
	{
		IUnknown_AddRef(rcox->factory);
		q->factory = rcox->factory;
		if(COM_S_OK != IUnknown_QueryInterface(rcox->factory, &IID_IClassFactory, (void **) &(q->cf)))
		{
			q->cf = NULL;
		}
	}
	else if(NULL != rcox->modulepath)
	{
		if(NULL == (q->modulepath = strdup(rcox->modulepath)))
		{
			com__registry_unlock();
			return COM_E_OUTOFMEMORY;
		}
	}
	r = COM_S_OK;
#ifdef COM_USE_XPCOM
	if(COM_S_OK != (r = com__xpcom_register(rcox, q->cf)))
	{
		return r;
	}
#endif
	/* Finalise the registration */
	if(r == COM_S_OK)
	{
		entrykey++;
		nentries++;
		*key = q->key;
	}
	com__registry_unlock();
	return r;
}

com_result_t
COM_SYM(com_unregister)(uint32_t key)
{
	size_t c;
	com_result_t r = COM_E_CLASSNOTREG;
	
	com__registry_lock();
	for(c = 0; c < nentries; c++)
	{
		if(key == entries[c].key)
		{
			r = com__unregister_entry(c);
			break;
		}
	}
	com__registry_unlock();
	return r;
}

com_result_t
COM_SYM(com_unregister_clsid)(com_rclsid_t clsid)
{
	size_t c;
	com_result_t r = COM_E_CLASSNOTREG;
	
	com__registry_lock();
	for(c = 0; c < nentries; c++)
	{
		if(com_guid_equal(clsid, &(entries[c].clsid)))
		{
			r = com__unregister_entry(c);
			break;
		}
	}
	com__registry_unlock();
	return r;
}

static com_result_t
com__getclass_entry(size_t index, com_server_t *server, com_riid_t riid, void **out)
{
	(void) server;
	
	if(NULL != entries[index].factory)
	{
		return IUnknown_QueryInterface(entries[index].factory, riid, out);
	}
#ifdef COM_USE_DL
	/* FIXME: Dynamic loading */
#endif
	return COM_E_DLLNOTFOUND;
}

com_result_t
COM_SYM(com_getclass)(com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out)
{
	com_result_t r;
	size_t c;
	
	r = COM_E_CLASSNOTREG;
	/* Check the internal registry first */
	if(0 != (context & COM_CTX_LOCALREG))
	{
		com__registry_lock();
		for(c = 0; c < nentries; c++)
		{
			if(com_guid_equal(clsid, &(entries[c].clsid)) && 0 != (entries[c].ctx & context & COM_CTX_LOCALREG))
			{
				r = com__getclass_entry(c, NULL, riid, out);
				break;
			}
		}
		com__registry_unlock();
	}
	if(COM_E_CLASSNOTREG != r)
	{
		return r;
	}
#ifdef COM_USE_XPCOM
	r = com__xpcom_getclass(clsid, context, server, riid, out);
	if(COM_E_CLASSNOTREG != r)
	{
		return r;
	}
#endif
	return r;
}

static void
com__registry_lock(void)
{
#ifdef COM_USE_PTHREAD
	pthread_mutex_lock(&registrylock);
#endif
}

static void
com__registry_unlock(void)
{
#ifdef COM_USE_PTHREAD
	pthread_mutex_unlock(&registrylock);
#endif
}

static com_result_t
com__unregister_entry(size_t c)
{
#ifdef COM_USE_XPCOM
	if(NULL != entries[c].cf)
	{
		com__xpcom_unregister(&(entries[c].clsid), entries[c].cf);
		IClassFactory_Release(entries[c].cf);
	}
#endif
	if(NULL != entries[c].factory)
	{
		IUnknown_Release(entries[c].factory);
	}
	free(entries[c].modulepath);
	memmove(&entries[c], &(entries[c + 1]), sizeof(registry_entry_t) * (nentries - c - 1));
	nentries--;
	return COM_S_OK;
}

