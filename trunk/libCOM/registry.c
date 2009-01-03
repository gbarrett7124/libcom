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
	int incomplete;
	uint32_t key;
	size_t nkeys;
	uint32_t *keys;
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
static com_result_t com__resume(com_rco_t *rco);

/* FIXME: Support for COM_REG_SUSPENDED */
com_result_t
COM_SYM(com_register)(com_rco_t *rcox, uint32_t *key)
{
	ICoRegistry **providers;
	size_t nproviders, c;
	com_result_t r = COM_E_ACCESSDENIED;
	registry_entry_t *q;
	int success;
	
	com__tryinit();
	/* If there's no CLSID, a factory instance is meaningless */
	if(NULL == rcox->clsid)
	{
		rcox->factory = NULL;
	}
	/* If there's no factory and no filename, we can't do anything */
	if(NULL == rcox->factory && NULL == rcox->modulepath)
	{
		return COM_E_INVALIDARG;
	}
	/* If there's no filename and persistent registration is requested,
	 * fail as we can't fulfil that request.
	 */
	if(NULL == rcox->modulepath && COM_REG_PERSISTENT == (rcox->flags & COM_REG_PERSISTENT))
	{
		return COM_E_INVALIDARG;
	}
	/* If there's a filename but no CLSID, dynamically load the module and
	 * request self-registration.
	 */
	if(NULL == rcox->clsid && NULL != rcox->modulepath)
	{
		return com__selfreg_path(rcox->modulepath, rcox->flags);
	}
	/* We now either have:
	 * 1. A CLSID and a factory instance
	 * 2. A CLSID and a filename
	 * 3. Both
	 * We can now pass this on to the registry providers to do something
	 * useful with. If all of the providers return COM_E_NOT_SUPPORTED,
	 * then that's what we'll return.
	 */
	providers = com__reglist_get(&nproviders);
	if(0 == nproviders)
	{
		return COM_E_NOT_SUPPORTED;
	}
	com__registry_lock();
	
	if(NULL == (q = realloc(entries, sizeof(registry_entry_t) * (nentries + 1))))
	{
		com__registry_unlock();
		com__reglist_release(providers, nproviders);
		return COM_E_OUTOFMEMORY;
	}
	entries = q;
	q = &(entries[nentries]);
	memset(q, 0, sizeof(registry_entry_t));
	if(NULL == (q->keys = (uint32_t *) calloc(nproviders, sizeof(uint32_t))))
	{
		com__registry_unlock();
		com__reglist_release(providers, nproviders);
		return COM_E_OUTOFMEMORY;
	}
	q->incomplete = 1;
	if(NULL != key)
	{
		*key = q->key = entrykey;
	}
	nentries++;
	entrykey++;
	com__registry_unlock();
	q->nkeys = nproviders;
	success = 0;
	for(c = 0; c < nproviders; c++)
	{
		if(NULL != providers[c])
		{
			r = ICoRegistry_RegisterClass(providers[c], rcox, &(q->keys[c]));
			if(COM_S_OK == r)
			{
				success++;
			}
			else if(COM_E_NOT_SUPPORTED != r)
			{
				/* Unwind here */
				com__reglist_release(providers, nproviders);
				return r;
			}
		}
	}
	com__reglist_release(providers, nproviders);
	q->incomplete = 0;
	if(0 == success)
	{
		return COM_E_NOT_SUPPORTED;
	}
	return COM_S_OK;
}

com_result_t
COM_SYM(com_unregister)(uint32_t key)
{
/*	size_t c;
	
	com__tryinit();
	com__registry_lock();
	for(c = 0; c < nentries; c++)
	{
		if(0 != entries[c].incomplete)
		{
			continue;
		}
		if(key == entries[c].key)
		{
			entries[c].incomplete = 1;
			com__registry_unlock();
			return com__unregister_entry(c);
		}
	}
	com__registry_unlock(); */
	return COM_E_CLASSNOTREG;
}

com_result_t
COM_SYM(com_unregister_clsid)(com_rclsid_t clsid)
{
/*	size_t c;
	
	com__registry_lock();
	for(c = 0; c < nentries; c++)
	{
		if(0 != entries[c].incomplete)
		{
			continue;
		}
		if(com_guid_equal(clsid, &(entries[c].rco.clsid)))
		{
			com__registry_unlock();
			return com__unregister_entry(c);
		}
	}
	com__registry_unlock(); */
	return COM_E_CLASSNOTREG;
}

com_result_t
COM_SYM(com_getclass)(com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out)
{
	com_result_t r;
	size_t nproviders, c;
	ICoRegistry **providers;
	
	com__tryinit();
	providers = com__reglist_get(&nproviders);
	if(0 != nproviders)
	{
		for(c = 0; c < nproviders; c++)
		{
			if(NULL != providers[c])
			{
				r = ICoRegistry_GetClassObject(providers[c], clsid, context, server, riid, out);
				if(COM_E_CLASSNOTREG != r)
				{
					com__reglist_release(providers, nproviders);
					return r;
				}
			}
		}
	}
	com__reglist_release(providers, nproviders);
	return COM_E_CLASSNOTREG;
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
com__unregister_entry(size_t index)
{
	return COM_S_OK;
}

static com_result_t
com__resume(com_rco_t *rco)
{
	return COM_S_OK;
}
