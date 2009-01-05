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

#ifndef P_LIBCOM_H_
# define P_LIBCOM_H_                   1

# if defined(COM_USE_WIN32)
#  include "p_win32.h"
# endif

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# endif
# ifdef HAVE_PTHREAD_H
#  include <pthread.h>
# endif
# ifdef HAVE_DLFCN_H
#  include <dlfcn.h>
# endif
# if defined(COM_USE_CF)
#  include <CoreFoundation/CoreFoundation.h>
# endif
# include "COM/COM.h"

/* We need the local registry regardless of the others */
# define COM_USE_LOCALREG              1

typedef union
{
	com_guid_t guid;
	uuid_t uuid;
} guid_uuid_t;


COM_EXTERNC void com__tryinit(void);

ICoRegistry **com__reglist_get(size_t *count);
void com__reglist_release(ICoRegistry **list, size_t count);

com_result_t com__selfreg_path(const char *path, com_regflags_t flags);

void *com__dlopen(const char *path);
void *com__dlsym(void *h, const char *sym);
void com__dlclose(void *h);

# ifdef COM_USE_XPCOM 
COM_EXTERNC com_result_t com__xpcom_init(void);
COM_EXTERNC int com__xpcom_registry_init(void);
COM_EXTERNC int com__xpcom_taskmem_init(void);
COM_EXTERNC com_result_t com__xpcom_shutdown(void);
COM_EXTERNC com_result_t com__xpcom_register(com_rco_t *rcox);
COM_EXTERNC com_result_t com__xpcom_unregister(com_rclsid_t clsid, IClassFactory *factory);
COM_EXTERNC com_result_t com__xpcom_getclass(com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out);
# endif

/* Registry providers */
# ifdef COM_USE_LOCALREG
COM_EXTERNC com_result_t com__registry_local_init(void);
# endif
# ifdef COM_USE_WIN32
COM_EXTERNC com_result_t com__registry_win32_init(void);
# endif
# ifdef COM_USE_XPCOM
COM_EXTERNC com_result_t com__registry_xpcom_init(void);
# endif
# ifdef COM_USE_CF
COM_EXTERNC com_result_t com__registry_cfprefs_init(void);
# endif

#endif /* !P_LIBCOM_H_ */