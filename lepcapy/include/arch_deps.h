#ifndef LEPCAPY_ARCH_DEPS_H
#define LEPCAPY_ARCH_DEPS_H

#if defined(__amd64__) || defined(__x86_64__) || defined(__i386__) || defined(_X86_)
    #include <stdint.h>
    #include <string.h>
    #undef __LEPCAPY_ARCH_X86__
    #define __LEPCAPY_ARCH_X86__

    /*
     * Arch-deps atomic features
     */
    #define LOCK_PREFIX "\n\tlock; "

    #if defined(__amd64__) || defined(__x86_64__)
    #undef __LEPCAPY_ARCH_X86_64__
    #define __LEPCAPY_ARCH_X86_64__
    /*
     * x86 Atomic_64bit
     */
    typedef struct {
        uint64_t cnt;
    }atomic64_t;

    #define atomic64_get(atomic_val)\
        *(__volatile__ uint64_t *)(atomic_val)

    #define atomic64_mov(atomic_val, mov_val)\
        __asm__ __volatile__ ("movq %1, %0"\
                              : "+m" ((atomic_val)->cnt)\
                              : "ir" (mov_val))

    #define atomic64_add(atomic_val, add_val)\
        __asm__ __volatile__ (LOCK_PREFIX "addq %1, %0"\
                             : "+m" ((atomic_val)->cnt)\
                             : "ir" (add_val))

    #define atomic64_sub(atomic_val, sub_val)\
        __asm__ __volatile__ (LOCK_PREFIX "subq %1, %0"\
                              : "+m" ((atomic_val)->cnt)\
                              : "ir" (sub_val))

    #define atomic64_inc(atomic_val)\
        __asm__ __volatile__ (LOCK_PREFIX "incq %0"\
                              : "+m" ((atomic_val)->cnt))

    #define atomic64_dec(atomic_val)\
        __asm__ __volatile__ (LOCK_PREFIX "decq %0"\
                              : "+m" ((atomic_val)->cnt))

    __attribute__((always_inline)) static inline int atomic64_cmpxchg(atomic64_t *atomic_val, uint64_t old_val, uint64_t new_val){
        register unsigned char __ret asm("al");
        __volatile__ uint64_t *__ptr = (__volatile__ uint64_t *)&atomic_val->cnt;

        __asm__ __volatile__ (LOCK_PREFIX "cmpxchgq %2, %1"
                              "\t\nxor %%rax, %%rax"
                              "\t\nsete %0"
                              : "=a" (__ret), "=m" (*__ptr)
                              : "r" (new_val), "m" (*__ptr), "a" (old_val)
                              : "memory");
        return __ret;
    }
    #endif

    /*
     * x86 Atomic_32bit
     */
    typedef struct {
        uint32_t cnt;
    }atomic32_t;

    #define atomic32_get(atomic_val)\
        *(__volatile__ uint32_t *)(atomic_val)

    #define atomic32_mov(atomic_val, mov_val)\
        __asm__ __volatile__ ("movl %1, %0"\
                              : "+m" ((atomic_val)->cnt)\
                              : "ir" (mov_val))

    #define atomic32_add(atomic_val, add_val)\
            __asm__ __volatile__ (LOCK_PREFIX "addl %1, %0"\
                             : "+m" ((atomic_val)->cnt)\
                             : "ir" (add_val))

    #define atomic32_sub(atomic_val, sub_val)\
        __asm__ __volatile__ (LOCK_PREFIX "subl %1, %0"\
                              : "+m" ((atomic_val)->cnt)\
                              : "ir" (sub_val))

    #define atomic32_inc(atomic_val)\
        __asm__ __volatile__ (LOCK_PREFIX "incl %0"\
                              : "+m" ((atomic_val)->cnt))

    #define atomic32_dec(atomic_val)\
        __asm__ __volatile__ (LOCK_PREFIX "decl %0"\
                              : "+m" ((atomic_val)->cnt))

    __attribute__((always_inline)) static inline int atomic32_cmpxchg(atomic32_t *atomic_val, uint32_t old_val, uint32_t new_val){
        register unsigned char __ret asm("al");
        __volatile__ uint32_t *__ptr = (__volatile__ uint32_t *)&atomic_val->cnt;

        __asm__ __volatile__ (LOCK_PREFIX "cmpxchgl %2, %1"
                              "\t\nxor %%eax, %%eax"
                              "\t\nsete %0"
                              : "=a" (__ret), "=m" (*__ptr)
                              : "r" (new_val), "m" (*__ptr), "a" (old_val)
                              : "memory");
        return __ret;
    }

    /*
     * Deprecated custom AVX features
     */
    #define __fastcpy_aligned32(dest, src)\
        memcpy(&dest, &src, 32);

#endif //__LEPCAPY_ARCH_X86__ [defined(__amd64__) || defined(__x86_64__) || defined(__i386__) || defined(_X86_)]
#endif //LEPCAPY_ARCH_DEPS_H
