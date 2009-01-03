/* Automatically generated from types.idl by comidl */

/*
 * libcom: An implementation of the Component Object Model
 * @(#) $Id$
 */

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

# ifndef COM_COM_H_
#  error Do not include this file directly; use <COM/COM.h> instead.
# endif
#ifndef COMIDL_TYPES_H_
# define COMIDL_TYPES_H_

# include "COM/COM.h"

/* COMTypes version 0.0 */
# ifndef COMTypes_v0_0_defined_
#  define COMTypes_v0_0_defined_
#  undef RPC_EXPORTS
#  ifdef RPC_EXPORT_COMTypes_v0_0
#   define RPC_EXPORTS
#  endif
#  include "DCE-RPC/decl.h"

typedef struct com_guid_struct 
{
	uint32_t data1;
	uint16_t data2;
	uint16_t data3;
	uint8_t data4[8];
} com_guid_t, com_iid_t, com_clsid_t;
typedef int32_t com_result_t;
typedef uint8_t com_bool_t;
typedef uint32_t com_size_t;
# if defined(__cplusplus)
typedef const com_iid_t &com_riid_t;
typedef const com_clsid_t &com_rclsid_t;
# else
typedef const com_iid_t *com_riid_t;
typedef const com_clsid_t *com_rclsid_t;
#endif
typedef struct com_rco_struct com_rco_t;
typedef struct com_server_struct com_server_t;
typedef struct com_multiqi_struct com_multiqi_t;
typedef enum com_context_enum 
{
		COM_CTX_INPROC_SERVER = 1,
		COM_CTX_INPROC_HANDLER = 2,
		COM_CTX_LOCAL_SERVER = 4,
		COM_CTX_RESERVED1 = 8,
		COM_CTX_REMOTE_SERVER = 16,
		COM_CTX_INPROC = 3,
		COM_CTX_SERVER = 21,
		COM_CTX_ALL = 23
} com_context_t;
# undef DEFINE_GUID
# define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) RPC_EXTERNC const com_guid_t name
# if defined(__cplusplus)
#  define com_guid_equal(a, b) (!memcmp(&(a), &(b), sizeof(com_guid_t)))
# else
#  define com_guid_equal(a, b) (!memcmp(a, b, sizeof(com_guid_t)))
# endif

#  undef RPC_EXPORTS
# endif /*!COMTypes_v0_0_defined_*/


#endif /*!COMIDL_TYPES_H_*/
