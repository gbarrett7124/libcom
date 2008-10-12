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

#define COM_CINTERFACE
#include "p_libcom.h"

#ifdef COM_USE_XPCOM

# include <nsXPCOM.h>
# include <nsIMemory.h>

static com_result_t CoMallocXPCOM_QueryInterface(IMalloc *intf, com_riid_t iid, void **out);
static uint32_t CoMallocXPCOM_AddRef(IMalloc *intf);
static uint32_t CoMallocXPCOM_Release(IMalloc *intf);
static void *CoMallocXPCOM_Alloc(IMalloc *intf, uint32_t size);
static void CoMallocXPCOM_Free(IMalloc *intf, void *ptr);
static void *CoMallocXPCOM_Realloc(IMalloc *intf, void *ptr, uint32_t size);
static size_t CoMallocXPCOM_GetSize(IMalloc *intf, void *ptr);
static int CoMallocXPCOM_DidAlloc(IMalloc *intf, void *ptr);
static void CoMallocXPCOM_HeapMinimize(IMalloc *intf);

/* #include "taskmem_xpcom.h" */

DECLARE_CLASS(CoMallocXPCOM)
{
	DECLARE_IMPLEMENTS(CoMallocXPCOM, IMalloc);
	nsIMemory *allocator;
};

DEFINE_CLASS_INTERFACE(CoMallocXPCOM, IMalloc)
{
	CoMallocXPCOM_QueryInterface,
	CoMallocXPCOM_AddRef,
	CoMallocXPCOM_Release,
	CoMallocXPCOM_Alloc,
	CoMallocXPCOM_Realloc,
	CoMallocXPCOM_Free,
	CoMallocXPCOM_GetSize,
	CoMallocXPCOM_DidAlloc,
	CoMallocXPCOM_HeapMinimize	
};

DEFINE_STATIC_CLASS(CoMallocXPCOM, com_taskmem)
{
	DEFINE_STATIC_IMPLEMENTATION(CoMallocXPCOM, IMalloc),
	/* allocator: */ NULL
};

int
com__xpcom_taskmem_init(void)
{
	NS_GetMemoryManager(&(com_taskmem.allocator));
	return COM_S_OK;
}

IMalloc *
COM_SYM(com_allocator)(void)
{
	INITIALISE_ADDREF_INTERFACE_POINTER(CoMallocXPCOM, &com_taskmem, IMalloc);
	
	return GET_INTERFACE_POINTER(&com_taskmem, IMalloc);
}

com_result_t
COM_COMPAT(CoGetMalloc)(uint32_t context, IMalloc **out)
{
	if(1 != context)
	{
		return COM_E_INVALIDARG;
	}
	INITIALISE_ADDREF_INTERFACE_POINTER(CoMallocXPCOM, &com_taskmem, IMalloc);
	
	*out = GET_INTERFACE_POINTER(&com_taskmem, IMalloc);
	return COM_S_OK;
}

void *
COM_COMPAT(CoTaskMemAlloc)(uint32_t size)
{
	return NS_Alloc(size);
}

void
COM_COMPAT(CoTaskMemFree)(void *ptr)
{
	return NS_Free(ptr);
}

void *
COM_COMPAT(CoTaskMemRealloc)(void *ptr, uint32_t newsize)
{
	return NS_Realloc(ptr, newsize);
}


static com_result_t
CoMallocXPCOM_QueryInterface(IMalloc *intf, com_riid_t iid, void **out)
{
	if(com_guid_equal(iid, IID_IUnknown) || com_guid_equal(iid, IID_IMalloc))
	{
		IMalloc_AddRef(intf);
		*out = intf;
		return COM_S_OK;
	}
	return COM_E_NOINTERFACE;
}

static uint32_t
CoMallocXPCOM_AddRef(IMalloc *intf)
{
	(void) intf;
	
	return 2;
}

static uint32_t
CoMallocXPCOM_Release(IMalloc *intf)
{
	(void) intf;
	
	return 1;
}

static void *
CoMallocXPCOM_Alloc(IMalloc *intf, uint32_t size)
{
	(void) intf;
	
	return com_taskmem.allocator->Alloc(size);
}

static void
CoMallocXPCOM_Free(IMalloc *intf, void *ptr)
{
	(void) intf;
	
	return com_taskmem.allocator->Free(ptr);
}

static void *
CoMallocXPCOM_Realloc(IMalloc *intf, void *ptr, size_t size)
{
	(void) intf;

	return com_taskmem.allocator->Realloc(ptr, size);
}

static void
CoMallocXPCOM_HeapMinimize(IMalloc *intf)
{
	(void) intf;
	
	com_taskmem.allocator->HeapMinimize(true);
}

static size_t
CoMallocXPCOM_GetSize(IMalloc *intf, void *ptr)
{
	(void) intf;
	(void) ptr;
	
	return (size_t) -1;
}

static int
CoMallocXPCOM_DidAlloc(IMalloc *intf, void *ptr)
{
	(void) intf;
	(void) ptr;
	
	return (size_t) -1;
}

#endif /* COM_USE_XPCOM */
