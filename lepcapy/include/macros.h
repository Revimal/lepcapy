#ifndef LEPCAPY_MACROS_H
#define LEPCAPY_MACROS_H

/*
 * Gloabal Dependencies
 */
#include <stdint.h>
#include <stdlib.h>

/*
 * Compile Option
 */
#define MAX_QUEUE_SIZE 4096
#define NETIO_QUEUING_SIZE 1024
#define NETIO_QUEING_WARN 64

/*
 * General Macros
 */
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    #include <sys/param.h>
#endif

#ifdef _MSC_VER
    #define LEPCAPY_DEBUG
    #define LEPCAPY_EXPLICIT
    #define LEPCAPY_SECTION(name)
    #define LEPCAPY_ALIGN(size) __declspec(align (size))
    #define LEPCAPY_PACK_DEF __pragma(pack(push, 1))
            #define LEPCAPY_PACK __pragma(pack(pop))
    #define LEPCAPY_DEPRECATED(func) func __declspec(deprecated)
    #define LEPCAPY_LFUNC inline
    #define LEPCAPY_NOEXCEPT noexcept
    #define LEPCAPY_EXPECT_T(x)
    #define LEPCAPY_EXPECT_F(x)
#else
    #define LEPCAPY_DUMMY __attribute__ ((unused))
    #define LEPCAPY_EXPLICIT __attribute__ ((used))
    #define LEPCAPY_SECTION(name) __attribute__ ((section (name)))
    #define LEPCAPY_ALIGN(size) __attribute__ ((aligned (size)))
    #define LEPCAPY_PACK_DEF
    #define LEPCAPY_PACK __attribute__ ((packed))
    #define LEPCAPY_DEPRECATED(func) func __attribute__ ((deprecated))
    #define LEPCAPY_LFUNC inline
    #define LEPCAPY_NOEXCEPT noexcept
    #define LEPCAPY_EXPECT_T(x) __builtin_expect((x), 1)
    #define LEPCAPY_EXPECT_F(x) __builtin_expect((x), 0)
#endif

/*
 * Error Control Macros
 */
#define SUCCESS         0
#define EPERM           1   /* Operation not permitted */
#define EFIO            2   /* File I/O error */
#define ETRANS          3   /* Network transportation error */
#define ENULL           4   /* Null pointer access fault */
#define EADDR           5   /* Misc address access fault */
#define EACCES          6   /* Permission denied */
#define EINVAL          7   /* Invalid arguments */
#define E2BIG           8   /* Argument too long */
#define EMAGIC          9   /* Magic check fault */
#define EQUEUE          10  /* Record queue fault */
#define ETHREAD         10  /* Threading fault */
#define EOVRFLW         11  /* Overflow fault */
#define EIOCTL          12  /* Ioctl Error */
#define ESOCK           13  /* Socket Error */
#define EINVPF          14  /* Invalid | Non-support protocol features */
#define EPMINIT         15  /* Initialize error on pktm */
#define __EDROP         16  /* Dropped packet */
#define EINTRACT        17  /* Thread interaction error */
#define EJFRAME         18  /* Not support Jumbo-frame */

/*
 * General Macro Functions
 */
#define alloc_type(p_var, d_type)                       \
    (p_var = (d_type *)malloc(sizeof(d_type)))

#define alloc_contig(p_var, d_type, cnt)                \
    (p_var = (d_type *)malloc(sizeof(d_type) * cnt))

#define free_ptr(p_var)                                 \
    do{                                                 \
        if(p_var != NULL){                              \
                free(p_var);                            \
                p_var = NULL;                           \
        }                                               \
    }while(0)                                           \

#endif // LEPCAPY_MACROS_H
