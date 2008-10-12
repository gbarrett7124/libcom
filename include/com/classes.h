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

struct com_classptr_struct
{
	IUnknown *lpVtbl;
	void *self;
};

# define DECLARE_CLASS(__classname) \
	typedef struct __classname __classname; \
	struct __classname

# define DECLARE_IMPLEMENTS(__classname, __intf) \
	struct { \
		const struct __intf##Vtbl *lpVtbl; \
		struct __classname *self; \
	} __intf##_iptr;

# define SELF(__classname, __intf)  \
	(struct __classname *)(((struct com_classptr_struct *) __intf)->self)
	
# define DECLARE_SELF(__classname, __intf) \
	struct __classname *self = SELF(__classname, __intf);

# define DEFINE_CLASS_INTERFACE(__classname, __intf) \
	static const struct __intf##Vtbl __classname##_##__intf =

# define INITIALISE_INTERFACE_POINTER(__instance, __intf) \
	if(NULL == (__instance)->__intf##_iptr.self) \
	{ \
		(__instance)->__intf##_iptr.self = (__instance); \
	}

# define DEFINE_STATIC_CLASS(__classname, __instname) \
	static struct __classname __instname =
	
# define DEFINE_STATIC_IMPLEMENTATION(__classname, __intf) \
	{ &__classname##_##__intf, NULL }

# define GET_INTERFACE_POINTER(__instance, __intf) \
	(__intf *)(&((__instance)->__intf##_iptr))

#endif /* !COM_INTERFACES_H_ */
