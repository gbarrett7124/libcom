/* Automatically generated from api.idl by comidl */

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

#ifndef COMIDL_API_H_
# define COMIDL_API_H_

# include "COM/COM.h"

/* CoAPI version 0.0 */
# ifndef CoAPI_v0_0_defined_
#  define CoAPI_v0_0_defined_
#  undef RPC_EXPORTS
#  ifdef RPC_EXPORT_CoAPI_v0_0
#   define RPC_EXPORTS
#  endif
#  include "DCE-RPC/decl.h"

typedef enum com_regflags_enum
{
	 	COM_REG_SINGLEUSE = 0,
	 	COM_REG_MULTIPLEUSE = (1<<0),
	 	COM_REG_MULTI_SEPARATE = (1<<1),
	 	COM_REG_SUSPENDED = (1<<2),
	 	COM_REG_SURROGATE = (1<<3),
	 	COM_REG_PERSISTENT = (1<<30),
	 	COM_REG_ALLUSERS = (1<<31)
}  com_regflags_t;
typedef enum com_guidgen_enum
{
	 	COM_GUIDGEN_DEFAULT = 0,
	 	COM_GUIDGEN_TIME = 1,
	 	COM_GUIDGEN_RANDOM = 2
}  com_guidgen_t;
typedef enum com_guidstr_enum
{
	 	COM_GUIDSTR_DEFAULT = 0,
	 	COM_GUIDSTR_UPPERCASE = (1<<0),
	 	COM_GUIDSTR_BRACES = (1<<1)
}  com_guidstr_t;
struct com_rco_struct
{
	const com_clsid_t *clsid;
	IUnknown *factory;
	com_context_t ctx;
	com_regflags_t flags;
	const char *displayname;
	const char *contractid;
	const char *modulepath;
} ;
struct com_server_struct
{
	int dummy;
} ;
struct com_multiqi_struct
{
	const com_iid_t *pIID;
	IUnknown *pItf;
	com_result_t hr;
} ;
RPC_CEXPORT com_result_t RPC_SYM(com_init)(const char *rdn) RPC_ALIAS(com_init);
RPC_CEXPORT com_result_t RPC_SYM(com_shutdown)(void) RPC_ALIAS(com_shutdown);
RPC_CEXPORT com_result_t RPC_SYM(com_registry_add)(ICoRegistry *registry) RPC_ALIAS(com_registry_add);
RPC_CEXPORT com_result_t RPC_SYM(com_registry_remove)(ICoRegistry *registry) RPC_ALIAS(com_registry_remove);
RPC_CEXPORT com_result_t RPC_SYM(com_getclass)(com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out) RPC_ALIAS(com_getclass);
RPC_CEXPORT com_result_t RPC_SYM(com_register)(com_rco_t *rcox, uint32_t *key) RPC_ALIAS(com_register);
RPC_CEXPORT com_result_t RPC_SYM(com_register_factory)(com_rclsid_t clsid, com_context_t context, const char *contractid, IUnknown *factory, uint32_t *key) RPC_ALIAS(com_register_factory);
RPC_CEXPORT com_result_t RPC_SYM(com_register_inprocsrv)(com_context_t context, const char *path, uint32_t *key) RPC_ALIAS(com_register_inprocsrv);
RPC_CEXPORT com_result_t RPC_SYM(com_unregister)(uint32_t key) RPC_ALIAS(com_unregister);
RPC_CEXPORT com_result_t RPC_SYM(com_guid_generate)(com_guid_t *out, com_guidgen_t flags) RPC_ALIAS(com_guid_generate);
RPC_CEXPORT com_result_t RPC_SYM(com_guid_to_string)(const com_guid_t *guid, char *buf, com_size_t buflen, com_guidstr_t options) RPC_ALIAS(com_guid_to_string);
RPC_CEXPORT IMalloc *RPC_SYM(com_allocator)(void) RPC_ALIAS(com_allocator);

#  ifdef RPC_ALIAS_MACROS
#   define com_init RPC_LOCAL_SYM(com_init)
#   define com_shutdown RPC_LOCAL_SYM(com_shutdown)
#   define com_registry_add RPC_LOCAL_SYM(com_registry_add)
#   define com_registry_remove RPC_LOCAL_SYM(com_registry_remove)
#   define com_getclass RPC_LOCAL_SYM(com_getclass)
#   define com_register RPC_LOCAL_SYM(com_register)
#   define com_register_factory RPC_LOCAL_SYM(com_register_factory)
#   define com_register_inprocsrv RPC_LOCAL_SYM(com_register_inprocsrv)
#   define com_unregister RPC_LOCAL_SYM(com_unregister)
#   define com_guid_generate RPC_LOCAL_SYM(com_guid_generate)
#   define com_guid_to_string RPC_LOCAL_SYM(com_guid_to_string)
#   define com_allocator RPC_LOCAL_SYM(com_allocator)
#  endif /*RPC_ALIAS_MACROS*/

#  undef RPC_EXPORTS
# endif /*!CoAPI_v0_0_defined_*/


#endif /*!COMIDL_API_H_*/
