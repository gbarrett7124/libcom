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

#ifdef COM_USE_PTHREAD
static pthread_mutex_t initlock = PTHREAD_MUTEX_INITIALIZER;
static pthread_key_t initkey;
static uint32_t initdummy = 0xFEEDFACE;
#endif

static inline int
com__init_global(void)
{
	/* initlock should already be held at this point */
#ifdef COM_USE_PTHREAD
	if(initkey) return 0;
	pthread_key_create(&initkey, NULL);
#endif
	return 1;
}

static inline void
com__init_perthread(void)
{
#ifdef COM_USE_PTHREAD
	if(NULL != pthread_getspecific(initkey))
	{
		return;
	}
#endif
#ifdef COM_USE_WIN32
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif
#ifdef COM_USE_PTHREAD
	pthread_setspecific(initkey, &initdummy);
#endif
}

void
com__tryinit(void)
{
	int r;
	
	pthread_mutex_lock(&initlock);
	r = com__init_global();
	com__init_perthread();
	pthread_mutex_unlock(&initlock);
	if(r)
	{
		/* One-time initialiation */
#ifdef COM_USE_LOCALREG
		com__registry_local_init();
#endif
#ifdef COM_USE_WIN32
		com__registry_win32_init();
#endif
#ifdef COM_USE_XPCOM
		com__registry_xpcom_init();
#endif
#ifdef COM_USE_CFPLUGIN
		com__registry_cfplugin_init();
#endif
	}
}

/* Initialise COM for the current thread */
com_result_t
COM_SYM(com_init)(const char *appname)
{
	com__tryinit();
	return COM_S_OK;
}

com_result_t
COM_SYM(com_shutdown)(void)
{
	return COM_S_OK;
}

com_result_t
COM_COMPAT(CoInitialize)(void *reserved)
{
	(void) reserved;
	
	com__tryinit();
	return COM_S_OK;
}

com_result_t
COM_COMPAT(CoInitializeEx)(void *reserved, uint32_t flags)
{
	(void) reserved;
	(void) flags;
	
	com__tryinit();
	return COM_S_OK;
}
