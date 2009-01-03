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

/*! \file
 * \addtogroup functions Function reference
 * \addtogroup win32 Win32/Win64 compatibility
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
#ifdef COM_USE_CF
		com__registry_cfprefs_init();
#endif
	}
}

/**
 * @brief Initialise COM for the current thread.
 * @param[in] identifier An optional application identifier in reverse-DNS (Java-style/UTI) format.
 * @return On success, @c COM_S_OK is returned. On error, an appropriate \c com_result_t value is returned.
 * @headerfile com.h <com/com.h>
 * @ingroup functions
 *
 * \p com_init() explicitly performs initialisation necessary to use COM in a
 * thread, optionally specifying an application identifier which causes
 * a private per-application registry to be used instead of the global
 * COM registry in environments where this is supported.
 *
 * If \a identifier is specified and \p com_init() has previously been called,
 * it will be ignored.
 *
 * If \p com_init() is not explicitly called for a thread, the equivalent of
 * \p com_init(NULL) will be invoked automatically.
 *
 * @note On Win32/Win64, \p com_init() calls \p CoInitializeEx(NULL, COINIT_MULTITHREADED)
 *
 * @sa com_result_t, com_shutdown()
 */
 
com_result_t
COM_SYM(com_init)(const char *identifier)
{
	com__tryinit();
	return COM_S_OK;
}

/**
 * @brief Release resources retained by COM in the current thread.
 * @return On success, @c COM_S_OK is returned.
 * @headerfile com.h <com/com.h>
 * @ingroup functions
 *
 * \p com_shutdown() releases resources maintained by the COM runtime during
 * normal operation. You should call \p com_shutdown() for each thread which
 * uses COM functions in order to prevent resource leaks.
 *
 * @sa com_init()
 */

com_result_t
COM_SYM(com_shutdown)(void)
{
	return COM_S_OK;
}

/**
 * @brief Initialise COM for the current thread.
 * @return On success, \c COM_S_OK is returned.
 * @headerfile com.h <com/com.h>
 * @ingroup win32
 *
 * Calling \p CoInitialize() is the equivalent of calling \p com_init(NULL)
 *
 * @sa com_init(), CoInitializeEx()
 */

com_result_t
COM_COMPAT(CoInitialize)(void *reserved)
{
	(void) reserved;
	
	com__tryinit();
	return COM_S_OK;
}

/**
 * @brief Initialise COM for the current thread.
 * @param[in] reserved Ignored.
 * @param[in] flags Ignored.
 * @return On success, \c COM_S_OK is returned.
 * @headerfile com.h <com/com.h>
 * @ingroup win32
 *
 * Calling \p CoInitializeEx() is the equivalent of calling \p com_init(NULL)
 *
 * @note Under Win32/Win64’s native COM implementation, various values can be
 * passed as the \p flags parameter. libcom always ignores the value of
 * \p flags and behaves as though \p flags was set to \p COINIT_MULTITHREADED.
 *
 * @sa com_init(), CoInitialize()
 */

com_result_t
COM_COMPAT(CoInitializeEx)(void *reserved, uint32_t flags)
{
	(void) reserved;
	(void) flags;
	
	com__tryinit();
	return COM_S_OK;
}
