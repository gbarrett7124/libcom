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

/* The XPCOM and Win32 task allocators take precedence over ours */
#if !defined(COM_USE_XPCOM) && !defined(COM_USE_WIN32)

# define COTASKMEM_SIGNATURE            0xCAFECAFE

static com_result_t CoMalloc_QueryInterface(IMalloc *intf, com_riid_t iid, void **out);
static uint32_t CoMalloc_AddRef(IMalloc *intf);
static uint32_t CoMalloc_Release(IMalloc *intf);
static void *CoMalloc_Alloc(IMalloc *intf, com_size_t size);
static void CoMalloc_Free(IMalloc *intf, void *ptr);
static void *CoMalloc_Realloc(IMalloc *intf, void *ptr, com_size_t size);
static com_size_t CoMalloc_GetSize(IMalloc *intf, void *ptr);
static int CoMalloc_DidAlloc(IMalloc *intf, void *ptr);
static void CoMalloc_HeapMinimize(IMalloc *intf);

/* #include "taskmem_xpcom.h" */

DECLARE_CLASS(CoMalloc)
{
	DECLARE_IMPLEMENTS(CoMalloc, IMalloc);
};

DEFINE_CLASS_INTERFACE(CoMalloc, IMalloc)
{
	CoMalloc_QueryInterface,
	CoMalloc_AddRef,
	CoMalloc_Release,
	CoMalloc_Alloc,
	CoMalloc_Realloc,
	CoMalloc_Free,
	CoMalloc_GetSize,
	CoMalloc_DidAlloc,
	CoMalloc_HeapMinimize	
};

DEFINE_STATIC_CLASS(CoMalloc, com_taskmem)
{
	DEFINE_STATIC_IMPLEMENTATION(CoMalloc, IMalloc),
};

IMalloc *
COM_SYM(com_allocator)(void)
{
	INITIALISE_ADDREF_INTERFACE_POINTER(CoMalloc, &com_taskmem, IMalloc);
	
	return GET_INTERFACE_POINTER(&com_taskmem, IMalloc);
}

com_result_t
COM_COMPAT(CoGetMalloc)(uint32_t context, IMalloc **out)
{
	if(1 != context)
	{
		return COM_E_INVALIDARG;
	}
	INITIALISE_ADDREF_INTERFACE_POINTER(CoMalloc, &com_taskmem, IMalloc);
	
	*out = GET_INTERFACE_POINTER(&com_taskmem, IMalloc);
	return COM_S_OK;
}

void *
COM_COMPAT(CoTaskMemAlloc)(com_size_t size)
{
	return CoMalloc_Alloc(NULL, size);
}

void
COM_COMPAT(CoTaskMemFree)(void *ptr)
{
	CoMalloc_Free(NULL, ptr);
}

void *
COM_COMPAT(CoTaskMemRealloc)(void *ptr, com_size_t newsize)
{
	return CoMalloc_Realloc(NULL, ptr, newsize);
}

static com_result_t
CoMalloc_QueryInterface(IMalloc *intf, com_riid_t iid, void **out)
{
	if(com_guid_equal(iid, &IID_IUnknown) || com_guid_equal(iid, &IID_IMalloc))
	{
		IMalloc_AddRef(intf);
		*out = intf;
		return COM_S_OK;
	}
	return COM_E_NOINTERFACE;
}

static uint32_t
CoMalloc_AddRef(IMalloc *intf)
{
	(void) intf;
	
	return 2;
}

static uint32_t
CoMalloc_Release(IMalloc *intf)
{
	(void) intf;
	
	return 1;
}

static void *
CoMalloc_Alloc(IMalloc *intf, com_size_t size)
{
	uint32_t *p;

	(void) intf;
	
	if(NULL == (p = (uint32_t *) malloc(size + (sizeof(uint32_t) * 2))))
	{
		return NULL;
	}
	p[0] = COTASKMEM_SIGNATURE;
	p[1] = size;
	return &(p[2]);
}

static void
CoMalloc_Free(IMalloc *intf, void *ptr)
{
	uint32_t *p = (uint32_t *) ptr;
	
	(void) intf;

	if(NULL == ptr)
	{
		return;
	}
	p -= 2;
	if(COTASKMEM_SIGNATURE != p[0])
	{
		return;
	}
	free(p);
}

static void *
CoMalloc_Realloc(IMalloc *intf, void *ptr, com_size_t size)
{
	uint32_t *p = (uint32_t *) ptr, *q;
	
	(void) intf;

	if(NULL != ptr)
	{
		p -= 2;
		if(COTASKMEM_SIGNATURE != p[0])
		{
			return NULL;
		}
	}
	if(NULL != (q = realloc(p, size + (sizeof(uint32_t) * 2))))
	{
		q[1] = size;
		return &(q[2]);
	}
	return NULL;
}

static void
CoMalloc_HeapMinimize(IMalloc *intf)
{
	(void) intf;
}

static com_size_t
CoMalloc_GetSize(IMalloc *intf, void *ptr)
{
	uint32_t *p = (uint32_t *) ptr;
	
	(void) intf;

	if(NULL == ptr)
	{
		return (uint32_t) -1;
	}
	p -= 2;
	if(COTASKMEM_SIGNATURE != p[0])
	{
		return (uint32_t) -1;
	}
	return p[1];
}

static int
CoMalloc_DidAlloc(IMalloc *intf, void *ptr)
{
	uint32_t *p = (uint32_t *) ptr;
	
	(void) intf;

	if(NULL == ptr)
	{
		return -1;
	}
	p -= 2;
	if(COTASKMEM_SIGNATURE != p[0])
	{
		return 0;
	}
	return 1;
}

#endif /* !COM_USE_XPCOM && !COM_USE_WIN32 */
