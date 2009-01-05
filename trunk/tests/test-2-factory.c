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

#include "COM/COM.h"
#include "ITester2.h"
#include "Test2.h"
#include "Test2Factory.h"
#include "test-2-guids.h"

DEFINE_GENERIC_FACTORY(Test2Factory, { return new_Test2(outer, riid, out, 1234); })


int
test2_server_init(void)
{
	IUnknown *server = getTest2Factory();
	com_result_t r;
	uint32_t regkey;
	
	if(COM_S_OK != (r = com_register_factory(&CLSID_Test2, COM_CTX_LOCAL_SERVER, NULL, server, &regkey)))
	{
		fprintf(stderr, "Failed to register factory for local server. Result = 0x%08x\n", r);
		exit(EXIT_FAILURE);
	}
	IUnknown_Release(server);
	return 0;
}
