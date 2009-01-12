/* Automatically generated from compat.idl by comidl */

/* @(#) $Id$ */

/*
 * Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009 Mo McRoberts.
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

#ifndef COMIDL_COMPAT_H_
# define COMIDL_COMPAT_H_

# include "COM/COM.h"

/* CoCompat version 0.0 */
# ifndef CoCompat_v0_0_defined_
#  define CoCompat_v0_0_defined_
#  undef RPC_EXPORTS
#  ifdef RPC_EXPORT_CoCompat_v0_0
#   define RPC_EXPORTS
#  endif
#  include "DCE-RPC/decl.h"

typedef void *LPVOID;
RPC_CEXPORT com_result_t RPC_SYM(CoInitialize)(LPVOID reserved) RPC_ALIAS(CoInitialize);
RPC_CEXPORT com_result_t RPC_SYM(CoInitializeEx)(LPVOID reserved, uint32_t flags) RPC_ALIAS(CoInitializeEx);
RPC_CEXPORT com_result_t RPC_SYM(CoRegisterClassObject)(com_rclsid_t clsid, IUnknown *factory, com_context_t context, com_regflags_t flags, uint32_t *key) RPC_ALIAS(CoRegisterClassObject);
RPC_CEXPORT com_result_t RPC_SYM(CoRevokeClassObject)(uint32_t key) RPC_ALIAS(CoRevokeClassObject);
RPC_CEXPORT com_result_t RPC_SYM(CoGetClassObject)(com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out) RPC_ALIAS(CoGetClassObject);
RPC_CEXPORT com_result_t RPC_SYM(CoCreateInstanceEx)(com_rclsid_t clsid, IUnknown *outer, com_context_t context, com_server_t *server, com_size_t intfcount, com_multiqi_t *intf) RPC_ALIAS(CoCreateInstanceEx);
RPC_CEXPORT com_result_t RPC_SYM(CoCreateInstance)(com_rclsid_t clsid, IUnknown *outer, com_context_t ctx, com_riid_t riid, void **out) RPC_ALIAS(CoCreateInstance);
RPC_CEXPORT com_result_t RPC_SYM(CoGetMalloc)(uint32_t context, IMalloc **out) RPC_ALIAS(CoGetMalloc);
RPC_CEXPORT void *RPC_SYM(CoTaskMemAlloc)(com_size_t size) RPC_ALIAS(CoTaskMemAlloc);
RPC_CEXPORT void RPC_SYM(CoTaskMemFree)(LPVOID optr) RPC_ALIAS(CoTaskMemFree);
RPC_CEXPORT void *RPC_SYM(CoTaskMemRealloc)(LPVOID optr, com_size_t newsize) RPC_ALIAS(CoTaskMemRealloc);
/* COINIT values - COINIT_MULITHREADED is all we support */
#  define COINIT_MULTITHREADED         0

/* CLSCTX values */
#  define CLSCTX_INPROC_SERVER         COM_CTX_INPROC_SERVER
#  define CLSCTX_INPROC_HANDLER        COM_CTX_INPROC_HANDLER
#  define CLSCTX_LOCAL_SERVER          COM_CTX_LOCAL_SERVER
#  define CLSCTX_REMOTE_SERVER         COM_CTX_REMOTE_SERVER

/* Other aliases */
#  define IsEqualGuid(a, b)            com_guid_equal(a, b)

#  ifdef RPC_ALIAS_MACROS
#   define CoInitialize RPC_LOCAL_SYM(CoInitialize)
#   define CoInitializeEx RPC_LOCAL_SYM(CoInitializeEx)
#   define CoRegisterClassObject RPC_LOCAL_SYM(CoRegisterClassObject)
#   define CoRevokeClassObject RPC_LOCAL_SYM(CoRevokeClassObject)
#   define CoGetClassObject RPC_LOCAL_SYM(CoGetClassObject)
#   define CoCreateInstanceEx RPC_LOCAL_SYM(CoCreateInstanceEx)
#   define CoCreateInstance RPC_LOCAL_SYM(CoCreateInstance)
#   define CoGetMalloc RPC_LOCAL_SYM(CoGetMalloc)
#   define CoTaskMemAlloc RPC_LOCAL_SYM(CoTaskMemAlloc)
#   define CoTaskMemFree RPC_LOCAL_SYM(CoTaskMemFree)
#   define CoTaskMemRealloc RPC_LOCAL_SYM(CoTaskMemRealloc)
#  endif /*RPC_ALIAS_MACROS*/

#  undef RPC_EXPORTS
# endif /*!CoCompat_v0_0_defined_*/


#endif /*!COMIDL_COMPAT_H_*/
