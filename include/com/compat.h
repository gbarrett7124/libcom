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

#ifndef COM_COMPAT_H_
# define COM_COMPAT_H_                 1

# ifndef COM_COM_H_
#  error Do not include this file directly; use <com/com.h> instead.
# endif

COM_EXPORT com_result_t COM_COMPAT(CoRegisterClassObject)(com_rclsid_t clsid, IUnknown *factory, com_context_t context, com_regflags_t flags, uint32_t *key);
COM_EXPORT com_result_t COM_COMPAT(CoRevokeClassObject)(uint32_t key);
COM_EXPORT com_result_t COM_COMPAT(CoGetClassObject)(com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out);
COM_EXPORT com_result_t COM_COMPAT(CoCreateInstanceEx)(com_rclsid_t clsid, IUnknown *outer, com_context_t context, com_server_t *server, size_t intfcount, com_multiqi_t *intf);
COM_EXPORT com_result_t COM_COMPAT(CoCreateInstance)(com_rclsid_t clsid, IUnknown *outer, com_context_t ctx, com_riid_t riid, void **out);

COM_EXPORT com_result_t COM_COMPAT(CoGetMalloc)(com_context_t context, IMalloc **out);
COM_EXPORT void *COM_COMPAT(CoTaskMemAlloc)(uint32_t size);
COM_EXPORT void COM_COMPAT(CoTaskMemFree)(void *ptr);
COM_EXPORT void *COM_COMPAT(CoTaskMemRealloc)(void *ptr, uint32_t newsize);

# if !defined(_WIN32) || defined(COM_USE_COMPAT)

#  define CoRegisterClassObject        COM_COMPAT(CoRegisterClassObject)
#  define CoRevokeClassObject          COM_COMPAT(CoRevokeClassObject)
#  define CoGetClassObject             COM_COMPAT(CoGetClassObject)
#  define CoCreateInstance             COM_COMPAT(CoCreateInstance)
#  define CoCreateInstanceEx           COM_COMPAT(CoCreateInstanceEx)
#  define CoGetMalloc                  COM_COMPAT(CoGetMalloc)
#  define CoTaskMemAlloc               COM_COMPAT(CoTaskMemAlloc)
#  define CoTaskMemFree                COM_COMPAT(CoTaskMemFree)
#  define CoTaskMemRealloc             COM_COMPAT(CoTaskMemRealloc)

# endif /* !WIN32 || COM_USE_COMPAT */

#endif /* !COM_COMPAT_H_ */