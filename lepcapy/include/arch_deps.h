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

__attribute__((always_inline))  inline void atomic64_rmov(atomic64_t *atomic_val, atomic64_t *mov_val){
#if defined(__amd64__) || defined(__x86_64__)
    __asm__ __volatile__ ("movq %1, %0"
                          : "+m" (atomic_val->cnt)
                          : "m" (mov_val->cnt));
#elif defined(__arm__)
#else
#endif
}

__attribute__((always_inline))  inline void atomic64_imov(atomic64_t *atomic_val, uint64_t mov_val){
#if defined(__amd64__) || defined(__x86_64__)
    __asm__ __volatile__ ("movq %1, %0"
                          : "+m" (atomic_val->cnt)
                          : "ir" (mov_val));
#elif defined(__arm__)
#else
#endif
}

__attribute__((always_inline))  inline void atomic64_iadd(atomic64_t *atomic_val, uint64_t add_val){
    #if defined(__amd64__) || defined(__x86_64__)
        __asm__ __volatile__ (LOCK_PREFIX "addq %1, %0"
                         : "+m" (atomic_val->cnt)
                         : "ir" (add_val));
    #elif defined(__arm__)
    #else
    #endif
}

__attribute__((always_inline))  inline void atomic64_isub(atomic64_t *atomic_val, uint64_t sub_val){
#if defined(__amd64__) || defined(__x86_64__)
    __asm__ __volatile__ (LOCK_PREFIX "subq %1, %0"
                          : "+m" (atomic_val->cnt)
                          : "ir" (sub_val));
#elif defined(__arm__)
#else
#endif
}

__attribute__((always_inline))  inline void atomic64_inc(atomic64_t *atomic_val){
#if defined(__amd64__) || defined(__x86_64__)
    __asm__ __volatile__ (LOCK_PREFIX "incq %0"
                          : "+m" (atomic_val->cnt));
#elif defined(__arm__)
#else
#endif
}

__attribute__((always_inline)) inline void atomic64_dec(atomic64_t *atomic_val){
#if defined(__amd64__) || defined(__x86_64__)
    __asm__ __volatile__ (LOCK_PREFIX "decq %0"
                          : "+m" (atomic_val->cnt));
#elif defined(__arm__)
#else
#endif
}

/*
 * Arch-deps optimized features
 */
#if defined(__AVX__)
    #include "immintrin.h"
#elif defined(__SSE3__)
#elif defined(__ARM_NEON__)
#else
    #include <string.h>
#endif

__attribute__((always_inline)) static inline void __fastcpy_aligned32(void *dest, void *src){
    #if defined(__AVX__)
        *(__m256i *)dest = _mm256_loadu_si256((__m256i *)src);
    #elif defined(__SSE3__)
    #elif defined(__ARM_NEON__)
    #else
        memcpy(dest, src, 32);
    #endif
}


__attribute__((always_inline)) static inline void __fastcpy_aligned32_wcmem(void *dest, void *src){
    #if defined(__AVX2__)
        *(__m256i *)dest = _mm256_stream_load_si256((__m256i *)src);
    #elif defined(__AVX__)
        *(__m256i *)dest = _mm256_loadu_si256((__m256i *)src);
    #elif defined(__SSE3__)
    #elif defined(__ARM_NEON__)
    #else
        memcpy(dest, src, 32);
    #endif
}

#endif //__LEPCAPY_ARCH_OPTS__

#endif //LEPCAPY_ARCH_DEPS_H
