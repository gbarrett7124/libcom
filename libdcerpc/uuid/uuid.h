/*
 * Public include file for the UUID library
 * 
 * Copyright (C) 1996, 1997, 1998 Theodore Ts'o.
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

#ifndef _UUID_UUID_H
#define _UUID_UUID_H

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#include "com/com.h"

#ifdef __cplusplus
extern "C" {
#endif

/* clear.c */
void com__uuid_clear(uuid_t uu);

/* compare.c */
int com__uuid_compare(const uuid_t uu1, const uuid_t uu2);

/* copy.c */
void com__uuid_copy(uuid_t dst, const uuid_t src);

/* gen_uuid.c */
void com__uuid_generate(uuid_t out);
void com__uuid_generate_random(uuid_t out);
void com__uuid_generate_time(uuid_t out);

/* isnull.c */
int com__uuid_is_null(const uuid_t uu);

/* parse.c */
int com__uuid_parse(const char *in, uuid_t uu);

/* unparse.c */
void com__uuid_unparse(const uuid_t uu, char *out);
void com__uuid_unparse_lower(const uuid_t uu, char *out);
void com__uuid_unparse_upper(const uuid_t uu, char *out);

/* uuid_time.c */
time_t com__uuid_time(const uuid_t uu, struct timeval *ret_tv);
int com__uuid_type(const uuid_t uu);
int com__uuid_variant(const uuid_t uu);

#ifdef __cplusplus
}
#endif

#endif /* _UUID_UUID_H */
