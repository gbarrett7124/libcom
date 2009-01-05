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

#define INITGUID
#include "test-2-guids.h"

static com_result_t __stdcall Test2_ITester2_QueryInterface(ITester2 *intf, com_riid_t iid, void **out);
static uint32_t __stdcall Test2_ITester2_AddRef(ITester2 *intf);
static uint32_t __stdcall Test2_ITester2_Release(ITester2 *intf);
static com_result_t __stdcall Test2_ITester2_Set(ITester2 *intf, int newval);
static int __stdcall Test2_ITester2_Get(ITester2 *intf);

DEFINE_CLASS_INTERFACE(Test2, ITester2)
{
	Test2_ITester2_QueryInterface,
	Test2_ITester2_AddRef,
	Test2_ITester2_Release,
	Test2_ITester2_Set,
	Test2_ITester2_Get
};

com_result_t
new_Test2(IUnknown *outer, com_rclsid_t riid, void **out, int defval)
{
	Test2 *p;
	ITester2 *q;
	com_result_t r;
	
	(void) outer;
	
	fprintf(stderr, "new_Test2: Allocating a new instance\n");
	if(NULL == (p = (Test2 *) calloc(1, sizeof(Test2))))
	{
		return COM_E_OUTOFMEMORY;
	}
	p->foo = defval;
	INITIALISE_ADDREF_INTERFACE_POINTER(Test2, p, ITester2);
	q = GET_INTERFACE_POINTER(p, ITester2);
	r = q->lpVtbl->QueryInterface(q, riid, out);
	q->lpVtbl->Release(q);
	return r;
}

static com_result_t __stdcall
Test2_ITester2_QueryInterface(ITester2 *intf, com_riid_t iid, void **out)
{
	if(com_guid_equal(iid, &IID_IUnknown) || com_guid_equal(iid, &IID_ITester2))
	{
		fprintf(stderr, "Test2::QueryInterface(): returning supported interface\n");
		intf->lpVtbl->AddRef(intf);
		*out = intf;
		return COM_S_OK;
	}
	fprintf(stderr, "Test2::QueryInterface(): interface is not supported\n");
	return COM_E_NOINTERFACE;
}

static uint32_t __stdcall
Test2_ITester2_AddRef(ITester2 *intf)
{
	DECLARE_SELF(Test2, ITester2, intf);
	
	self->refcount++;
	fprintf(stderr, "Test2::AddRef(): refcount is now %u\n", self->refcount);
	return self->refcount;
}

static uint32_t __stdcall
Test2_ITester2_Release(ITester2 *intf)
{
	DECLARE_SELF(Test2, ITester2, intf);
	
	if(1 == self->refcount)
	{
		fprintf(stderr, "Test2::Release(): refcount is now 0, freeing\n");
		free(self);
		return 0;
	}
	self->refcount--;
	fprintf(stderr, "Test2::Release(): refcount is now %u\n", self->refcount);
	return self->refcount;
}

static com_result_t __stdcall
Test2_ITester2_Set(ITester2 *intf, int newval)
{
	DECLARE_SELF(Test2, ITester2, intf);
	
	fprintf(stderr, "Test2::Set(): foo was %d, now %d\n", self->foo, newval);
	self->foo = newval;
	return COM_S_OK;
}

static int __stdcall
Test2_ITester2_Get(ITester2 *intf)
{
	DECLARE_SELF(Test2, ITester2, intf);
	
	return self->foo;
}
