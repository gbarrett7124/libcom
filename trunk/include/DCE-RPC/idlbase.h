#ifndef DCE_RPC_IDLBASE_H_
# define DCE_RPC_IDLBASE_H_

# include <stdint.h>
# include <sys/socket.h>

# ifndef TRUE
#  define TRUE                         1
# endif

# ifndef FALSE
#  define FALSE                        0
# endif

# if !defined(__cplusplus)
#  ifndef true
#   define true                        TRUE
#  endif
#  ifndef false
#   define false                       FALSE
#  endif
# endif

#endif /* !DCE_RPC_IDLBASE_H_ */