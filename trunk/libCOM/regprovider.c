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

/* These functions maintain the registry providers list: our registry APIs
 * are a proxy to one or more registry providers (implementing ICoRegistry),
 * which deal with the actual component registration/unregistration.
 * While libcom provides some stock implementations of ICoRegistry which are
 * added to the list of registry providers automatically, an application can
 * add its own ICoRegistry implementation, and thus take on the role of a
 * registry provider—this allows for an application to maintain its own
 * registry of components which are used in preference to the platform-supplied
 * implementations.
 */

static size_t nproviders;
static ICoRegistry **providers;

#ifdef COM_USE_PTHREAD
static pthread_mutex_t reglistlock = PTHREAD_MUTEX_INITIALIZER;
#endif

static void com__reglist_lock(void);
static void com__reglist_unlock(void);
static com_result_t com__reglist_add(ICoRegistry *registry);

/* com_registry_add()
 * Add an ICoRegistry implementation to the list of available providers.
 */
com_result_t
COM_SYM(com_registry_add)(ICoRegistry *registry)
{
	com_result_t r;
	
	com__tryinit();
	ICoRegistry_AddRef(registry);
	com__reglist_lock();
	r = com__reglist_add(registry);
	com__reglist_unlock();
	if(r != COM_S_OK)
	{
		/* We failed to add the provider; release it again */
		ICoRegistry_Release(registry);
	}
	return r;
}

static void
com__reglist_lock(void)
{
#ifdef COM_USE_PTHREAD
	pthread_mutex_lock(&reglistlock);
#endif
}

static void
com__reglist_unlock(void)
{
#ifdef COM_USE_PTHREAD
	pthread_mutex_unlock(&reglistlock);
#endif
}

static com_result_t
com__reglist_add(ICoRegistry *reg)
{
	ICoRegistry **q;
	
	if(NULL == (q = realloc(providers, sizeof(ICoRegistry *) * (nproviders + 2))))
	{
		return COM_E_OUTOFMEMORY;
	}
	providers = q;
	q[nproviders] = reg;
	nproviders++;
}

/* Return the list of registry providers, calling AddRef on each */
ICoRegistry **
com__reglist_get(size_t *count)
{
	size_t c;
	ICoRegistry **plist;
	
	com__reglist_lock();
	if(NULL != (plist = providers))
	{
		for(c = 0; c < nproviders; c++)
		{
			if(NULL != plist[c])
			{
				ICoRegistry_AddRef(plist[c]);
			}
		}
	}
	*count = nproviders;
	com__reglist_unlock();
	return plist;
}

/* Release a list of registry providers */
void
com__reglist_release(ICoRegistry **list, size_t count)
{
	size_t c;
	
	for(c = 0; c < count; c++)
	{
		if(NULL != list[c])
		{
			ICoRegistry_Release(list[c]);
		}
	}
}
