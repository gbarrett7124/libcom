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

#include <stdio.h>
#include <stdlib.h>

#define COM_COMPAT_MSCOM
#include "com/com.h"

#include "ITester2.h"
#include "test-2-guids.h"

extern int test2_server_init(void);

int
main(int argc, char **argv)
{
	IClassFactory *client;
	ITester2 *p;
	com_result_t r;
	int v;
	
	(void) argc;
	(void) argv;
	
	com_init("com.googlecode.libcom.test-2");
	
	test2_server_init();
		
	if(COM_S_OK != (r = com_getclass(&CLSID_Test2, COM_CTX_ALL, NULL, &IID_IClassFactory, (void **) &client)))
	{
		fprintf(stderr, "Failed to connect to registered factory. Result = 0x%08x\n", r);
		exit(EXIT_FAILURE);
	}
	if(COM_S_OK != (r = IClassFactory_CreateInstance(client, NULL, &IID_ITester2, (void **) &p)))
	{
		fprintf(stderr, "IClassFactory::CreateInstance() failed. Result = 0x%08x\n", r);
		exit(EXIT_FAILURE);
	}
	IClassFactory_Release(client);
	p->lpVtbl->Set(p, 5678);
	if(5678 != (v = p->lpVtbl->Get(p)))
	{
		fprintf(stderr, "ITester2::Get() returned an unexpected value (got = %d, expected = 5678)\n", v);
		exit(EXIT_FAILURE);
	}
	fprintf(stderr, "ITester2::Get() = %d\n", p->lpVtbl->Get(p));
	p->lpVtbl->Release(p);
	puts("PASS");
	return 0;
}
