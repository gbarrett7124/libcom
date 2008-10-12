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

#ifndef COM_CLASSES_H_
# define COM_CLASSES_H_                1

# ifndef COM_COM_H_
#  error Do not include this file directly; use <com/com.h> instead.
# endif

/* Helpers for implementing classes in C */

# define DECLARE_CLASS(__classname) \
	typedef struct __classname __classname; \
	struct __classname

# define DECLARE_IMPLEMENTS(__classname, __intf) \
	union __classname##__##__intf##_union { \
		IUnknown u; \
		__intf i; \
		struct { \
			const struct __intf##Vtbl *lpVtbl; \
			struct __classname *self; \
		} c; \
	} __intf##_;

# define DECLARE_GENERIC_FACTORY(__classname) \
	DECLARE_CLASS(__classname) \
	{ \
		DECLARE_IMPLEMENTS(__classname, IClassFactory) \
	}; \
	COM_EXTERNC IUnknown *get##__classname(void);

# define SELF(__classname, __intf, __intfptr)  \
	(((union __classname##__##__intf##_union *) (__intfptr))->c.self)

# define DECLARE_SELF(__classname, __intf, __intfptr) \
	struct __classname *self = SELF(__classname, __intf, __intfptr);

# define DEFINE_CLASS_INTERFACE(__classname, __intf) \
	static const struct __intf##Vtbl __classname##_##__intf =

# define INITIALISE_INTERFACE_POINTER(__classname, __instance, __intf) \
	if(NULL == (__instance)->__intf##_.i.lpVtbl) \
	{ \
		(__instance)->__intf##_.i.lpVtbl = &__classname##_##__intf; \
	} \
	if(NULL == (__instance)->__intf##_.c.self) \
	{ \
		(__instance)->__intf##_.c.self = (__instance); \
	}

# define INITIALISE_ADDREF_INTERFACE_POINTER(__classname, __instance, __intf) \
	INITIALISE_INTERFACE_POINTER(__classname, __instance, __intf) \
	(__instance)->__intf##_.i.lpVtbl->AddRef(&((__instance)->__intf##_.i))

# define DEFINE_STATIC_CLASS(__classname, __instname) \
	static struct __classname __instname =
	
# define DEFINE_STATIC_IMPLEMENTATION(__classname, __intf) \
	{ { &__classname##_##__intf } }

# define GET_INTERFACE_POINTER(__instance, __intf) \
	(&((__instance)->__intf##_.i))

# define GET_ALIASED_IUNKNOWN(__instance, __intf) \
	(&((__instance)->__intf##_.u))

# define DEFINE_GENERIC_FACTORY(__classname, constructor) \
	static com_result_t __classname##_IClassFactory_QueryInterface(IClassFactory *intf, com_riid_t riid, void **out) \
	{ \
		if(com_guid_equal(riid, &IID_IUnknown) || com_guid_equal(riid, &IID_IClassFactory)) \
		{ \
			IClassFactory_AddRef(intf); \
			*out = intf; \
			return COM_S_OK; \
		} \
		return COM_E_NOINTERFACE; \
	} \
	\
	static uint32_t __classname##_IClassFactory_AddRef(IClassFactory *intf) \
	{ \
		(void) intf; \
		\
		return 2; \
	} \
	\
	static uint32_t __classname##_IClassFactory_Release(IClassFactory *intf) \
	{ \
		(void) intf; \
		\
		return 1; \
	} \
	\
	static com_result_t __classname##_IClassFactory_CreateInstance(IClassFactory *intf, IUnknown *outer, com_riid_t riid, void **out) \
	{ \
		(void) intf; \
		constructor; \
		return COM_E_NOINTERFACE; \
	} \
	\
	static com_result_t __classname##_IClassFactory_LockServer(IClassFactory *intf, com_bool_t lock) \
	{ \
		(void) intf; \
		(void) lock; \
		\
		return COM_S_OK; \
	} \
	DEFINE_CLASS_INTERFACE(__classname, IClassFactory) \
	{ \
		__classname##_IClassFactory_QueryInterface, \
		__classname##_IClassFactory_AddRef, \
		__classname##_IClassFactory_Release, \
		__classname##_IClassFactory_CreateInstance, \
		__classname##_IClassFactory_LockServer \
	}; \
	DEFINE_STATIC_CLASS(__classname, static_##__classname) \
	{ \
		DEFINE_STATIC_IMPLEMENTATION(__classname, IClassFactory) \
	}; \
	\
	IUnknown *get##__classname(void) \
	{ \
		INITIALISE_ADDREF_INTERFACE_POINTER(__classname, &static_##__classname, IClassFactory); \
		return GET_ALIASED_IUNKNOWN(&static_##__classname, IClassFactory); \
	}

#endif /* !COM_INTERFACES_H_ */
