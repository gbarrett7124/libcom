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

#define INITGUID
#include "COM/guiddef.h"
#include "COM/IUnknown.h"
#include "COM/ICoRegistry.h"
#include "COM/IClassFactory.h"
#include "COM/IMalloc.h"

const com_guid_t GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 }};

com_result_t
COM_SYM(com_guid_generate)(com_guid_t *out, com_guidgen_t flags)
{
	unsigned32 status;
	guid_uuid_t *u = (guid_uuid_t *) out;

	(void) flags;
	
	uuid_create(&(u->uuid), &status);
	return COM_S_OK;
}

com_result_t
COM_SYM(com_guid_to_string)(const com_guid_t *guid, char *buf, size_t buflen, com_guidstr_t flags)
{
	char tmp[40];
	unsigned char *p;
	unsigned32 status;
	guid_uuid_t *u = (guid_uuid_t *) guid;
	
/*	if(flags & COM_GUIDSTR_UPPERCASE)
	{
		com__uuid_unparse_upper(guid, &(tmp[1]));
	}
	else
	{
		com__uuid_unparse(guid, &(tmp[1]));
	} */
	uuid_to_string(&(u->uuid), &p, &status);
		
	if(flags & COM_GUIDSTR_BRACES)
	{
		tmp[0] = '{';
		strcpy(&(tmp[1]), (const char *) p);
		tmp[37] = '}';
		tmp[38] = 0;
		strncpy(buf, tmp, buflen);
	}
	else
	{
		strncpy(buf, (const char *) p, buflen);
	}
	buf[buflen - 1] = 0;
	rpc_string_free(&p, &status);
	return COM_S_OK;
}
