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
#define MAX_QUEUE_SIZE 8192
#define NETIO_QUEUING_SIZE 4096
#define NETIO_BUFFER_SIZE 4

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
#define EFIO            1   /* File I/O error */
#define ETRANS          2   /* Network transportation error */
#define ENULL           3   /* Null pointer access fault */
#define EMEM            4   /* Memory fault */
#define EINVAL          5   /* Invalid arguments */
#define EF2BIG          6   /* File is too big */
#define EMAGIC          7   /* Magic check fault */
#define EQUEUE          8   /* Record queue fault */
#define ETHREAD         9   /* Threading fault */
#define EOVRFLW         10  /* Overflow fault */
#define EIOCTL          11  /* Ioctl Error */
#define ESOCK           12  /* Socket Error */
#define EINVPF          13  /* Invalid | Non-support protocol features */
#define EPMINIT         14  /* Initialize error on pktm */
#define __EDROP         15  /* Dropped packet */
#define EINTRACT        16  /* Thread interaction error */
#define EJFRAME         17  /* Not support Jumbo-frame */
#define __EEOF          18  /* End of file */

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
