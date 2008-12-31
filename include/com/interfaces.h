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

# undef FWD_DECLARE_INTERFACE
# undef DECLARE_INTERFACE
# undef DECLARE_INTERFACE_
# undef PURE
# undef THIS
# undef THIS_
# undef BEGIN_INTERFACE
# undef END_INTERFACE
# undef STDMETHOD
# undef STDMETHOD_

# if !defined(__OBJC__)
#  undef interface
#  define interface struct
# endif

# undef cominterface
# define cominterface struct

# undef COM_STDMETHODCALLTYPE
# define COM_STDMETHODCALLTYPE         __stdcall

# if defined(__cplusplus) && !defined(COM_CINTERFACE)
#  define FWD_DECLARE_INTERFACE(__intf) \
	cominterface __intf;
#  define PURE                         = 0
#  define THIS_                        /* */
#  define THIS                         void
#  define STDMETHOD(__name) \
	virtual com_result_t COM_STDMETHODCALLTYPE __name
#  define STDMETHOD_(__type, __name) \
	virtual __type COM_STDMETHODCALLTYPE __name
#  if defined(__GNUC__) && __GNUC__ < 3
#   define DECLARE_INTERFACE(__intf) \
	struct __attribute__((com_interface)) __intf
#   define DECLARE_INTERFACE_(__intf, __parent) \
	struct __attribute__((com_interface)) __intf : public __parent
#  else
#   define DECLARE_INTERFACE(__intf) \
	struct __intf
#   define DECLARE_INTERFACE_(__intf, __parent) \
	struct __intf : public __parent
#  endif /* __GNUC__ && __GNUC__ < 3 */
# else /* __cplusplus && !COM_CINTERFACE */
#  define FWD_DECLARE_INTERFACE(__intf) \
	cominterface __intf;
#  if defined(__cplusplus)
#   define DECLARE_INTERFACE(__intf) \
	typedef cominterface __intf { \
		const struct __intf##Vtbl *lpVtbl; \
	} __intf; \
	\
	struct __intf##Vtbl
#  else
#   define DECLARE_INTERFACE(__intf) \
	typedef cominterface __intf { \
		const struct __intf##Vtbl *lpVtbl; \
	} __intf; \
	typedef const struct __intf##Vtbl __intf##Vtbl; \
	\
	struct __intf##Vtbl
#  endif /* __cplusplus */
#  define DECLARE_INTERFACE_(__intf, __parent) \
	DECLARE_INTERFACE(__intf)
#  define PURE                          /* */
#  define THIS                          INTERFACE *This
#  define THIS_                         INTERFACE *This,
#  define STDMETHOD(__name)             com_result_t (COM_STDMETHODCALLTYPE *__name)
#  define STDMETHOD_(__type,__name)     __type (COM_STDMETHODCALLTYPE *__name)
# endif /* !__cplusplus || COM_CINTERFACE */

# define BEGIN_INTERFACE               /* */
# define END_INTERFACE                 /* */

#endif /* !COM_INTERFACES_H_ */
