/* @(#) $Id$ */

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

/* This header is deliberately not protected against multiple inclusion
 * because these macros should be re-defined each time this header is
 * included.
 */

#undef RPC_EXPORT
#undef RPC_SYM
#undef RPC_EXPORTED
#undef RPC_IMPORTED
#undef RPC_EXTERNC
#undef RPC_DECLAREC
#undef RPC_EXPORT
#undef RPC_CEXPORT

#if defined(RPC_SYM_PREFIX) && defined(RPC_SYM_SUFFIX)
# define RPC_SYM(x)                    RPC_SYM_PREFIX##x##RPC_SYM_SUFFIX
#elif defined(RPC_SYM_PREFIX)
# define RPC_SYM(x)                    RPC_SYM_PREFIX##x
#elif defined(RPC_SYM_SUFFIX)
# define RPC_SYM(x)                    x##RPC_SYM_SUFFIX
#else
# define RPC_SYM(x)                    x
#endif

#if defined(_WIN32)
# if defined(__GNUC__)
#  define RPC_EXPORTED                 __attribute__((dllexport))
#  define RPC_IMPORTED                 __attribute__((dllimport))
# else
#  define RPC_EXPORTED                 __declspec(dllexport)
#  define RPC_IMPORTED                 __declspec(dllimport)
# endif
#elif defined(__GNUC__)
# define RPC_EXPORTED                  __attribute__((visibility("default")))
# define RPC_IMPORTED                  /* */
#endif

#if defined(__cplusplus)
# define RPC_EXTERNC                   extern "C"
# define RPC_DECLAREC                  extern "C"
#else
# define RPC_EXTERNC                   extern
# define RPC_DECLAREC                  /* */
#endif

#ifdef RPC_EXPORTS
# define RPC_EXPORT                    RPC_EXPORTED
#else
# define RPC_EXPORT                    RPC_IMPORTED
#endif

#define RPC_CEXPORT                    RPC_EXTERNC RPC_EXPORT

#if !defined(__stdcall)
# if defined(__GNUC__)
#  define __stdcall                   __attribute__((stdcall))
# else
#  define __stdcall                    /* */
# endif
#endif

