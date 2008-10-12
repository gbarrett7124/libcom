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
#endif
static int initcount;

/* Initialise COM for the current thread */
com_result_t
COM_SYM(com_init)(const char *appname)
{
#ifdef COM_USE_PTHREAD
	pthread_mutex_lock(&initlock);
#endif
#ifdef COM_USE_WIN32
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#endif
	if(0 == initcount)
	{
#ifdef COM_USE_XPCOM
		com__xpcom_init();
#endif
	}
	initcount++;
#ifdef COM_USE_PTHREAD
	pthread_mutex_unlock(&initlock);
#endif
	return COM_S_OK;
}

com_result_t
COM_SYM(com_shutdown)(void)
{
#ifdef COM_USE_PTHREAD
	pthread_mutex_lock(&initlock);
#endif
#ifdef COM_USE_WIN32
	CoUnitialize();
#endif
	initcount--;
#ifdef COM_USE_XPCOM
	if(0 == initcount)
	{
		com__xpcom_shutdown();
	}
#endif
#ifdef COM_USE_PTHREAD
	pthread_mutex_unlock(&initlock);
#endif
	return COM_S_OK;
}

com_result_t
COM_COMPAT(CoInitialize)(void *reserved)
{
	(void) reserved;
	
	return com_init(NULL);
}

com_result_t
COM_COMPAT(CoInitializeEx)(void *reserved, uint32_t flags)
{
	(void) reserved;
	(void) flags;
	
	return com_init(NULL);
}
