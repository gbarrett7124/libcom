/*
 * DCE/RPC: UUID utilities
 * @(#) $Id$
 */

/*
 * Copyright (c) 2008, 2009 Mo McRoberts.
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

/*
 * Copyright (C) 1996, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, and the entire permission notice in its entirety,
 *    including the disclaimer of warranties.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ALL OF
 * WHICH ARE HEREBY DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF NOT ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * %End-Header%
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>
#include "uuidP.h"

#define UUCMP(u1,u2) if (u1 != u2) return((u1 < u2) ? -1 : 1);

signed32
RPC_SYM(uuid_compare)(/*[in]*/ uuid_p_t uuid1, /*[in]*/ uuid_p_t uuid2, /*[out]*/ unsigned32 *status)
{
	*status = uuid_s_ok;
	
	UUCMP(uuid1->time_low, uuid2->time_low);
	UUCMP(uuid1->time_mid, uuid2->time_mid);
	UUCMP(uuid1->time_hi_and_version, uuid2->time_hi_and_version);
	UUCMP(uuid1->clock_seq_hi_and_reserved, uuid2->clock_seq_hi_and_reserved);
	UUCMP(uuid1->clock_seq_low, uuid2->clock_seq_low);
	
	return memcmp(uuid1->node, uuid2->node, 6);
}

boolean32
RPC_SYM(uuid_equal)(/*[in]*/ uuid_p_t uuid1, /*[in]*/ uuid_p_t uuid2, /*[out]*/ unsigned32 *status)
{
	if(0 == uuid_compare(uuid1, uuid2, status))
	{
		return TRUE;
	}
	return FALSE;
}

#define UUCHK(u1) if (u1) return FALSE;

boolean32
RPC_SYM(uuid_is_nil)(/*[in]*/ uuid_p_t uuid, /*[out]*/ unsigned32 *status)
{
	uint8_t *cp;
	int i;
	
	*status = uuid_s_ok;
	
	UUCHK(uuid->time_low);
	UUCHK(uuid->time_mid);
	UUCHK(uuid->time_hi_and_version);
	UUCHK(uuid->clock_seq_hi_and_reserved);
	UUCHK(uuid->clock_seq_low);
	
	for(i = 0, cp = uuid->node; i < 6; i++)
	{
		UUCHK(*cp);
	}
	return TRUE;
}
