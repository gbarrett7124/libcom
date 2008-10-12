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

#ifndef COM_INTERFACES_H_
# define COM_INTERFACES_H_             1

# ifndef COM_COM_H_
#  error Do not include this file directly; use <com/com.h> instead.
# endif

/* Attempt to be vaguely compatible with header files written for
 * Windows.
 */

# define DECLARE_INTERFACE(__intf) \
	typedef struct __intf __intf; \
	typedef struct __intf##Vtbl __intf##Vtbl; \
	struct __intf { \
		const struct __intf##Vtbl *lpVtbl; \
	}; \
	\
	struct __intf##Vtbl

# define DECLARE_INTERFACE_(__intf, __parent) \
	DECLARE_INTERFACE(__intf)

# define PURE                          /* */
# define THIS                          INTERFACE *This
# define THIS_                         INTERFACE *This,

# define BEGIN_INTERFACE               /* */
# define END_INTERFACE                 /* */

# define STDMETHOD(__name)             com_result_t (*__name)
# define STDMETHOD_(__type,__name)     __type (*__name)

#endif /* !COM_INTERFACES_H_ */
