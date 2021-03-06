#ifndef LECAPY_EXCEPTION_CTRL_H
#define LECAPY_EXCEPTION_CTRL_H

#include "macros.h"
#include <stdio.h>

#define ERR_NULL(_at) "null_ptr_<"#_at">"
#define ERR_INVAL(_at) "invalid_val_<"#_at">"

#define ERR_CALL(_at) "call_<"#_at">"
#define ERR_CALL_MACRO(_at) "call_[macro]_<"#_at">"
#define ERR_CALL_LIBC(_at) "call_[libc]_<"#_at">"
#define ERR_CALL_INTERNAL(_at) "call_[internal]_<"#_at">"
#define ERR_CALL_PKTM(_pktm, _at) "call_[pktm:"#_pktm"]_<"#_at">"
#define ERR_CALL_PROTO(_proto, _at) "call_[proto:"#_proto"]_<"#_at">"

#define ERR_THREAD_WORK(_thread, _at) "threading_["#_thread"]_<"#_at">"
#define ERR_THREAD_INTERNAL_WORK(_thread, _at) "threading_[internal:"#_thread"]_<"#_at">"
#define ERR_THREAD_IWORK(_thread, _at) "threading_["#_thread"]_<internal:"#_at">"
#define ERR_THREAD_INTERNAL_IWORK(_thread, _at) "threading_[internal:"#_thread"]_<internal:"#_at">"
#define ERR_INTERACT(_at) "interact_<"#_at">"
#define ERR_LCK_ULCK(_at) "lock_unlock_<"#_at">"

#define ERR_IOCTL(_at) "ioctl_<"#_at">"
#define ERR_PROTO(_cause, _at) "protocol_err_["#_cause"]_<"#_at">"
#define ERR_SYSDEPS(_at) "sysdeps_<"#_at">"

#define ERR_UNK() "unkwown_err"

#ifndef NDEBUG
    #ifdef __KERNEL__
        #define __debug__prtn_io_cnt(_io_cnt)   \
            printk(KERN_DEBUG "#Current <"#_io_cnt"> : %lu\n\n", _io_cnt);
        #define __debug__chkpoint(_chkpoint)   \
            printk(KERN_DEBUG "#Debug Checkpoint - "#_chkpoint"\n\n");
        #define raise_except(_at, _err_no)                                                                  \
            printk(KERN_DEBUG "#Exception raised at %s\n\t*Line : %u\n\t*Function : %s\n\t*Additional : %s - %d\n\n",    \
                   __FILE__, __LINE__, __func__, _at, _err_no);
    #else
        #define __debug__prtn_io_cnt(_io_cnt)   \
            printf("#Current <"#_io_cnt"> : %lu\n\n", _io_cnt);
        #define __debug__chkpoint(_chkpoint)   \
            printf("#Debug Checkpoint - "#_chkpoint"\n\n");
        #define raise_except(_at, _err_no)                                                                  \
            printf("#Exception raised at %s\n\t*Line : %u\n\t*Function : %s\n\t*Additional : %s - %d\n\n",    \
                   __FILE__, __LINE__, __func__, _at, _err_no);
    #endif //__KERNEL__
#else
    #define __debug__prtn_io_cnt(_io_cnt)
    #define __debug__chkpoint(_chkpoint)
    #define raise_except(_at, _err_no)
#endif

#endif //LECAPY_EXCEPTION_CTRL_H
