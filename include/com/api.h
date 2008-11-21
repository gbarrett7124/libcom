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

#ifndef COM_API_H_
# define COM_API_H_                    1

# ifndef COM_COM_H_
#  error Do not include this file directly; use <com/com.h> instead.
# endif


typedef enum com_regflags_enum
{
	COM_REG_SINGLEUSE = 0,
	COM_REG_MULTIPLEUSE = (1<<0),
	COM_REG_MULTI_SEPARATE = (1<<1),
	COM_REG_SUSPENDED = (1<<2),
	COM_REG_SURROGATE = (1<<3),
	
	COM_REG_PERSISTENT = (1<<30), /* Publish registration information, if possible */
	COM_REG_ALLUSERS = (1<<31) /* Publish registration globally, if possible */
} com_regflags_t;

typedef com_result_t (*com_getclassobject_t)(com_rclsid_t clsid, com_riid_t riid, void **out);

struct com_rco_struct
{
	const com_clsid_t *clsid;
	IUnknown *factory;
	com_context_t ctx;
	com_regflags_t flags;
	const char *displayname;
	const char *contractid;
	const char *modulepath;
};

struct com_server_struct
{
	int dummy;
};

struct com_multiqi_struct
{
	/* Match member names in Microsoft's MULTI_QI structure */
	const com_iid_t *pIID;
	IUnknown *pItf;
	com_result_t hr;
};

extern const com_guid_t GUID_NULL;

COM_CEXPORT com_result_t COM_SYM(com_init)(const char *rdn);
COM_CEXPORT com_result_t COM_SYM(com_shutdown)(void);

COM_CEXPORT com_result_t COM_SYM(com_registry_add)(ICoRegistry *registry);
COM_CEXPORT com_result_t COM_SYM(com_registry_remove)(ICoRegistry *registry);

COM_CEXPORT com_result_t COM_SYM(com_getclass)(com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out);
COM_CEXPORT com_result_t COM_SYM(com_register)(com_rco_t *rcox, uint32_t *key);
COM_CEXPORT com_result_t COM_SYM(com_register_factory)(com_rclsid_t clsid, com_context_t context, const char *contractid, IUnknown *factory, uint32_t *key);
COM_CEXPORT com_result_t COM_SYM(com_register_inprocsrv)(com_context_t context, const char *path, uint32_t *key);
COM_CEXPORT com_result_t COM_SYM(com_unregister)(uint32_t key);

COM_CEXPORT IMalloc *COM_SYM(com_allocator)(void) COM_EXPORTED;

#endif /* !COM_API_H_ */
