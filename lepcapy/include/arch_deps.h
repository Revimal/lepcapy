#ifndef LEPCAPY_ARCH_DEPS_H
#define LEPCAPY_ARCH_DEPS_H

#if defined(__amd64__) || defined(__x86_64__) || defined(__arm__)

#include <stdint.h>
#define __LEPCAPY_ARCH_OPTS__

/*
 * Arch-deps atomic features
 */
#define LOCK_PREFIX "\n\tlock; "

typedef struct {
    uint64_t cnt;
}atomic64_t;

__attribute__((always_inline)) static inline void atomic64_mov(atomic64_t *atomic_val, uint64_t mov_val){
#if defined(__amd64__) || defined(__x86_64__)
    __asm__ __volatile__ ("movq %1, %0"
                          : "+m" (atomic_val->cnt)
                          : "ir" (mov_val));
#elif defined(__arm__)
#else
#endif
}

__attribute__((always_inline)) static inline void atomic64_add(atomic64_t *atomic_val, uint64_t add_val){
    #if defined(__amd64__) || defined(__x86_64__)
        __asm__ __volatile__ (LOCK_PREFIX "addq %1, %0"
                         : "+m" (atomic_val->cnt)
                         : "ir" (add_val));
    #elif defined(__arm__)
    #else
    #endif
}

__attribute__((always_inline)) static inline void atomic64_sub(atomic64_t *atomic_val, uint64_t sub_val){
#if defined(__amd64__) || defined(__x86_64__)
    __asm__ __volatile__ (LOCK_PREFIX "subq %1, %0"
                          : "+m" (atomic_val->cnt)
                          : "ir" (sub_val));
#elif defined(__arm__)
#else
#endif
}

__attribute__((always_inline)) static inline void atomic64_inc(atomic64_t *atomic_val){
#if defined(__amd64__) || defined(__x86_64__)
    __asm__ __volatile__ (LOCK_PREFIX "incq %0"
                          : "+m" (atomic_val->cnt));
#elif defined(__arm__)
#else
#endif
}

__attribute__((always_inline)) static inline void atomic64_dec(atomic64_t *atomic_val){
#if defined(__amd64__) || defined(__x86_64__)
    __asm__ __volatile__ (LOCK_PREFIX "decq %0"
                          : "+m" (atomic_val->cnt));
#elif defined(__arm__)
#else
#endif
}

__attribute__((always_inline)) static inline int atomic64_cmpxchg(atomic64_t *atomic_val, uint64_t old_val, uint64_t new_val){
#if defined(__amd64__) || defined(__x86_64__)

    __volatile__ unsigned char __ret = 0;
    __volatile__ uint64_t *__ptr = (__volatile__ uint64_t *)&atomic_val->cnt;

    __asm__ __volatile__ (LOCK_PREFIX "cmpxchgq %2, %1"
                          "\t\nsete %0"
                          : "=q" (__ret), "=m" (*__ptr)
                          : "r" (new_val), "m" (*__ptr), "a" (old_val)
                          : "memory");

    return __ret;
#elif defined(__arm__)
#else
#endif
}

/*
 * Arch-deps optimized features
 */
#if defined(__AVX2__)
    #include "immintrin.h"
#else
    #include <string.h>
#endif

__attribute__((always_inline)) static inline void __fastcpy_aligned32(void *dest, void *src){
    #if defined(__AVX2__)
        *(__m256i *)dest = _mm256_loadu_si256((__m256i *)src);
    #else
        memcpy(dest, src, 32);
    #endif
}


__attribute__((always_inline)) static inline void __fastcpy_aligned32_wcmem(void *dest, void *src){
    #if defined(__AVX2__)
        *(__m256i *)dest = _mm256_stream_load_si256((__m256i *)src);
    #else
        memcpy(dest, src, 32);
    #endif
}

#endif //__LEPCAPY_ARCH_OPTS__

#endif //LEPCAPY_ARCH_DEPS_H
