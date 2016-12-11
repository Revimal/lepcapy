#ifndef LEPCAPY_ARCH_DEPS_H
#define LEPCAPY_ARCH_DEPS_H

#if defined(__amd64__) || defined(__x86_64__) || defined(__i386__) || defined(_X86_)

#include <stdint.h>
#define __LEPCAPY_ARCH_X86__

/*
 * Arch-deps atomic features
 */
#define LOCK_PREFIX "\n\tlock; "


/*
 * x86 Atomic_64bit
 */
#if defined(__amd64__) || defined(__x86_64__)
typedef struct {
    uint64_t cnt;
}atomic64_t;

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
    __volatile__ unsigned char __ret = 0;
    __volatile__ uint64_t *__ptr = (__volatile__ uint64_t *)&atomic_val->cnt;

    __asm__ __volatile__ (LOCK_PREFIX "cmpxchgq %2, %1"
                          "\t\nsete %0"
                          : "=q" (__ret), "=m" (*__ptr)
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

//__attribute__((always_inline)) static inline void atomic32_mov(atomic32_t *atomic_val, uint32_t mov_val){
//    __asm__ __volatile__ ("movl %1, %0"
//                          : "+m" (atomic_val->cnt)
//                          : "ir" (mov_val));
//}

//__attribute__((always_inline)) static inline void atomic32_add(atomic32_t *atomic_val, uint32_t add_val){
//        __asm__ __volatile__ (LOCK_PREFIX "addl %1, %0"
//                         : "+m" (atomic_val->cnt)
//                         : "ir" (add_val));
//}

//__attribute__((always_inline)) static inline void atomic32_sub(atomic32_t *atomic_val, uint32_t sub_val){
//    __asm__ __volatile__ (LOCK_PREFIX "subl %1, %0"
//                          : "+m" (atomic_val->cnt)
//                          : "ir" (sub_val));
//}

//__attribute__((always_inline)) static inline void atomic32_inc(atomic32_t *atomic_val){
//    __asm__ __volatile__ (LOCK_PREFIX "incl %0"
//                          : "+m" (atomic_val->cnt));
//}

//__attribute__((always_inline)) static inline void atomic32_dec(atomic32_t *atomic_val){
//    __asm__ __volatile__ (LOCK_PREFIX "decl %0"
//                          : "+m" (atomic_val->cnt));
//}

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
    register __volatile__ unsigned char __ret asm("al");
    __volatile__ uint32_t *__ptr = (__volatile__ uint32_t *)&atomic_val->cnt;

    __asm__ __volatile__ (LOCK_PREFIX "cmpxchgl %2, %1"
                          "\t\nxor %%rax, %%rax"
                          "\t\nsete %0"
                          : "=a" (__ret), "=m" (*__ptr)
                          : "r" (new_val), "m" (*__ptr), "a" (old_val)
                          : "memory");
    return __ret;
}
//__attribute__((always_inline)) static inline int atomic32_cmpxchg(atomic32_t *atomic_val, uint32_t old_val, uint32_t new_val){
//    __volatile__ register unsigned char __ret = 0;
//    __volatile__ uint32_t *__ptr = (__volatile__ uint32_t *)&atomic_val->cnt;

//    __asm__ __volatile__ (LOCK_PREFIX "cmpxchgl %2, %1"
//                          "\t\nsete %0"
//                          : "=q" (__ret), "=m" (*__ptr)
//                          : "r" (new_val), "m" (*__ptr), "a" (old_val)
//                          : "memory");
//    return __ret;
//}

/*
 * Arch-deps optimized features
 */
#if defined(__AVX2__)
   #define __fastcpy_aligned32(dest, src)\
        __asm__ __volatile__("vmovdqa %1, %%ymm0;"\
                             "vmovdqa %%ymm0, %0"\
                             :"+m" (dest)\
                             :"m" (src)\
                             :"%ymm0");

    #define __fastcpy_aligned32_wcmem(dest, src)\
        __asm__ __volatile__("vmovntdqa %1, %%ymm0;"\
                             "vmovntdq %%ymm0, %0"\
                             :"+m" (dest)\
                             :"m" (src)\
                             :"%ymm0");

    #define __fastcpy_aligned32_enqueue(dest, src)\
        __asm__ __volatile__("vmovdqa %1, %%ymm0;"\
                             "vmovntdq %%ymm0, %0"\
                             :"+m" (dest)\
                             :"m" (src)\
                             :"%ymm0");

    #define __fastcpy_aligned32_dequeue(dest, src)\
        __asm__ __volatile__("vmovntdqa %1, %%ymm0;"\
                             "vmovdqa %%ymm0, %0"\
                             :"+m" (dest)\
                             :"m" (src)\
                             :"%ymm0");
#else
    #include <string.h>

    #define __fastcpy_aligned32(dest, src)\
        memcpy(&dest, &src, 32);

    #define __fastcpy_aligned32_wcmem(dest, src)\
        memcpy(&dest, &src, 32);

    #define __fastcpy_aligned32_enqueue(dest, src)\
        memcpy(&dest, &src, 32);

    #define __fastcpy_aligned32_dequeue(dest, src)\
        memcpy(&dest, &src, 32);

#endif
#endif //__LEPCAPY_ARCH_OPTS__
#endif //LEPCAPY_ARCH_DEPS_H
