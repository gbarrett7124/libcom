/* @(#) $Id$ */

#define RPC_SYM_PREFIX                 DCERPC
#include "DCE-RPC/decl.h"

RPC_EXTERNC void RPC_SYM(rpc__string_alloc)(size_t nbytes, unsigned_char_p_t *str, unsigned32 *status) RPC_ALIAS(rpc__string_alloc);

#ifdef RPC_ALIAS_MACROS
# define rpc__string_alloc             RPC_LOCAL_SYM(rpc__string_alloc)
#endif

#undef RPC_SYM_PREFIX
