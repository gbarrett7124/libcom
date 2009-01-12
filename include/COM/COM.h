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

#ifndef COM_COM_H_
# define COM_COM_H_                    1

# include <sys/types.h>
# include <inttypes.h>
# include <string.h>

# include "DCE-RPC/DCE-RPC.h"

# define RPC_SYM_PREFIX                COM

# include "COM/types.h"
# include "COM/error.h"
# include "COM/interfaces.h"

# include "COM/IUnknown.h"
# include "COM/ICoRegistry.h"
# include "COM/IClassFactory.h"
# include "COM/IMalloc.h"

# include "COM/classes.h"
# include "COM/api.h"
# include "COM/compat.h"

RPC_EXTERNC const com_guid_t RPC_SYM(GUID_NULL) RPC_ALIAS(GUID_NULL);

RPC_EXTERNC com_result_t __stdcall com_self_register(const char *pathname, com_regflags_t flags);
RPC_EXTERNC com_result_t __stdcall com_self_unregister(void);
RPC_EXTERNC com_result_t __stdcall com_self_getclass(com_rclsid_t rclsid, com_riid_t riid, void **out);
RPC_EXTERNC com_result_t __stdcall com_self_lockcount(void);

#endif /* !COM_COM_H_ */
