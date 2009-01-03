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

#if !defined(RTLD_LOCAL)
# define RTLD_LOCAL                    0
#endif

com_result_t
com__selfreg_path(const char *path, com_regflags_t flags)
{
	void *h;
	com_self_register_t selfreg;
	com_result_t r;
	
	if(NULL == (h = com__dlopen(path)))
	{
		return -1;
	}
	if(NULL == (selfreg = (com_self_register_t) com__dlsym(h, "com_self_register")))
	{
		fprintf(stderr, "com__selfreg_path: DLL does not export self-registration function\n");
		com__dlclose(h);
		return -1;
	}
	r = selfreg(path, flags);
	com__dlclose(h);
	return r;
}

void *
com__dlopen(const char *path)
{
#if defined(COM_USE_DL)
	return dlopen(path, RTLD_LOCAL|RTLD_NOW);
#endif
	return NULL;
}

void
com__dlclose(void *handle)
{
#if defined(COM_USE_DL)
	dlclose(handle);
#endif
}

void *
com__dlsym(void *handle, const char *symbol)
{
#if defined(COM_USE_DL)
	return dlsym(handle, symbol);
#endif
	return NULL;
}
