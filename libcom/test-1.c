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

#include "com/com.h"

int
main(int argc, char **argv)
{
	void *p;
	com_result_t r;
	int n;
	size_t s;
	IMalloc *allocator;
	
	(void) argc;
	(void) argv;
	
	com_init();
	
	p = CoTaskMemAlloc(64);
	if(NULL == p)
	{
		fprintf(stderr, "Failed to allocate 64 bytes via CoTaskMemAlloc()\n");
		exit(EXIT_FAILURE);
	}
	if(COM_S_OK != (r = CoGetMalloc(1, &allocator)))
	{
		fprintf(stderr, "Failed to obtain reference to task allocator; result = 0x%08x\n", r);
		exit(EXIT_FAILURE);
	}
	if(1 != (n = IMalloc_DidAlloc(allocator, p)))
	{
		if(0 == n)
		{
			fprintf(stderr, "IMalloc::DidAlloc() claims task allocator was not responsible for allocated block\n");
		}
		else
		{
			fprintf(stderr, "IMalloc::DidAlloc() could not determine responsibility for allocated block\n");
		}
		exit(EXIT_FAILURE);
	}
	if(64 != (s = IMalloc_GetSize(allocator, p)))
	{
		fprintf(stderr, "IMalloc::GetSize() returned an incorrect size (%ul bytes)\n", s);
		exit(EXIT_FAILURE);
	}
	IMalloc_Free(allocator, p);
	puts("PASS");
	
	com_shutdown();
	return 0;
}
