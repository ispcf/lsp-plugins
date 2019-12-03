/*
 * mix.h
 *
 *  Created on: 17 мар. 2017 г.
 *      Author: sadko
 */

#ifndef DSP_ARCH_X86_SSE_MIX_H_
#define DSP_ARCH_X86_SSE_MIX_H_

#ifndef DSP_ARCH_X86_SSE_IMPL
    #error "This header should not be included directly"
#endif /* DSP_ARCH_X86_SSE_IMPL */

namespace sse
{
    void mix2(float *a, const float *b, float k1, float k2, size_t count)
    {
        IF_ARCH_X86(size_t off);
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       %[k1], %%xmm6")
            __ASM_EMIT("movss       %[k2], %%xmm7")
            __ASM_EMIT("xor         %[off], %[off]")
            __ASM_EMIT("shufps      $0x00, %%xmm6, %%xmm6")
            __ASM_EMIT("shufps      $0x00, %%xmm7, %%xmm7")
            // 12x blocks
            __ASM_EMIT("sub         $12, %[count]")
            __ASM_EMIT("jb          2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x20(%[a],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm4")
            __ASM_EMIT("movups      0x20(%[b],%[off]), %%xmm5")
            __ASM_EMIT("mulps       %%xmm6, %%xmm0")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm1")
            __ASM_EMIT("mulps       %%xmm7, %%xmm4")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm5")
            __ASM_EMIT("addps       %%xmm3, %%xmm0")
            __ASM_EMIT("addps       %%xmm4, %%xmm1")
            __ASM_EMIT("addps       %%xmm5, %%xmm2")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[a],%[off])")
            __ASM_EMIT("movups      %%xmm2, 0x20(%[a],%[off])")
            __ASM_EMIT("add         $0x30, %[off]")
            __ASM_EMIT("sub         $12, %[count]")
            __ASM_EMIT("jae         1b")
            // 8x block
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          4f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm4")
            __ASM_EMIT("mulps       %%xmm6, %%xmm0")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm1")
            __ASM_EMIT("mulps       %%xmm7, %%xmm4")
            __ASM_EMIT("addps       %%xmm3, %%xmm0")
            __ASM_EMIT("addps       %%xmm4, %%xmm1")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[a],%[off])")
            __ASM_EMIT("add         $0x20, %[off]")
            __ASM_EMIT("sub         $8, %[count]")
            // 4x block
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          6f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm0")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm3, %%xmm0")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("add         $0x10, %[off]")
            __ASM_EMIT("sub         $4, %[count]")
            // 1x blocks
            __ASM_EMIT("6:")
            __ASM_EMIT("add         $3, %[count]")
            __ASM_EMIT("jl          8f")
            __ASM_EMIT("7:")
            __ASM_EMIT("movss       0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movss       0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulss       %%xmm6, %%xmm0")
            __ASM_EMIT("mulss       %%xmm7, %%xmm3")
            __ASM_EMIT("addss       %%xmm3, %%xmm0")
            __ASM_EMIT("movss       %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("add         $0x04, %[off]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jge         7b")
            // End
            __ASM_EMIT("8:")
            : [count] "+r" (count),
              [off] "=&r" (off)
            : [a] "r" (a), [b] "r" (b),
              [k1] "m" (k1), [k2] "m" (k2)
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void mix_copy2(float *dst, const float *a, const float *b, float k1, float k2, size_t count)
    {
        IF_ARCH_X86(size_t off);
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       %[k1], %%xmm6")
            __ASM_EMIT("movss       %[k2], %%xmm7")
            __ASM_EMIT("xor         %[off], %[off]")
            __ASM_EMIT("shufps      $0x00, %%xmm6, %%xmm6")
            __ASM_EMIT("shufps      $0x00, %%xmm7, %%xmm7")
            // 12x blocks
            __ASM_EMIT("sub         $12, %[count]")
            __ASM_EMIT("jb          2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x20(%[a],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm4")
            __ASM_EMIT("movups      0x20(%[b],%[off]), %%xmm5")
            __ASM_EMIT("mulps       %%xmm6, %%xmm0")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm1")
            __ASM_EMIT("mulps       %%xmm7, %%xmm4")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm5")
            __ASM_EMIT("addps       %%xmm3, %%xmm0")
            __ASM_EMIT("addps       %%xmm4, %%xmm1")
            __ASM_EMIT("addps       %%xmm5, %%xmm2")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst],%[off])")
            __ASM_EMIT("movups      %%xmm2, 0x20(%[dst],%[off])")
            __ASM_EMIT("add         $0x30, %[off]")
            __ASM_EMIT("sub         $12, %[count]")
            __ASM_EMIT("jae         1b")
            // 8x block
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          4f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm4")
            __ASM_EMIT("mulps       %%xmm6, %%xmm0")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm1")
            __ASM_EMIT("mulps       %%xmm7, %%xmm4")
            __ASM_EMIT("addps       %%xmm3, %%xmm0")
            __ASM_EMIT("addps       %%xmm4, %%xmm1")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst],%[off])")
            __ASM_EMIT("add         $0x20, %[off]")
            __ASM_EMIT("sub         $8, %[count]")
            // 4x block
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          6f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm0")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm3, %%xmm0")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x10, %[off]")
            __ASM_EMIT("sub         $4, %[count]")
            // 1x blocks
            __ASM_EMIT("6:")
            __ASM_EMIT("add         $3, %[count]")
            __ASM_EMIT("jl          8f")
            __ASM_EMIT("7:")
            __ASM_EMIT("movss       0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movss       0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulss       %%xmm6, %%xmm0")
            __ASM_EMIT("mulss       %%xmm7, %%xmm3")
            __ASM_EMIT("addss       %%xmm3, %%xmm0")
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x04, %[off]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jge         7b")
            // End
            __ASM_EMIT("8:")
            : [count] "+r" (count),
              [off] "=&r" (off)
            : [dst] "r" (dst), [a] "r" (a), [b] "r" (b),
              [k1] "m" (k1), [k2] "m" (k2)
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void mix_add2(float *dst, const float *a, const float *b, float k1, float k2, size_t count)
    {
        IF_ARCH_X86(size_t off);
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       %[k1], %%xmm6")
            __ASM_EMIT("movss       %[k2], %%xmm7")
            __ASM_EMIT("xor         %[off], %[off]")
            __ASM_EMIT("shufps      $0x00, %%xmm6, %%xmm6")
            __ASM_EMIT("shufps      $0x00, %%xmm7, %%xmm7")
            // 8x blocks
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jb          2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm3")
            __ASM_EMIT("movups      0x00(%[dst],%[off]), %%xmm4")
            __ASM_EMIT("movups      0x10(%[dst],%[off]), %%xmm5")
            __ASM_EMIT("mulps       %%xmm6, %%xmm0")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("mulps       %%xmm6, %%xmm1")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm0, %%xmm4")
            __ASM_EMIT("addps       %%xmm1, %%xmm5")
            __ASM_EMIT("addps       %%xmm2, %%xmm4")
            __ASM_EMIT("addps       %%xmm3, %%xmm5")
            __ASM_EMIT("movups      %%xmm4, 0x00(%[dst],%[off])")
            __ASM_EMIT("movups      %%xmm5, 0x10(%[dst],%[off])")
            __ASM_EMIT("add         $0x20, %[off]")
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jae         1b")
            // 4x block
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          4f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x00(%[dst],%[off]), %%xmm4")
            __ASM_EMIT("mulps       %%xmm6, %%xmm0")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("addps       %%xmm0, %%xmm4")
            __ASM_EMIT("addps       %%xmm2, %%xmm4")
            __ASM_EMIT("movups      %%xmm4, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x10, %[off]")
            __ASM_EMIT("sub         $4, %[count]")
            // 1x blocks
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $3, %[count]")
            __ASM_EMIT("jl          6f")
            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movss       0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movss       0x00(%[dst],%[off]), %%xmm4")
            __ASM_EMIT("mulss       %%xmm6, %%xmm0")
            __ASM_EMIT("mulss       %%xmm7, %%xmm2")
            __ASM_EMIT("addss       %%xmm0, %%xmm4")
            __ASM_EMIT("addss       %%xmm2, %%xmm4")
            __ASM_EMIT("movss       %%xmm4, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x04, %[off]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jge         5b")
            // End
            __ASM_EMIT("6:")
            : [count] "+r" (count),
              [off] "=&r" (off)
            : [dst] "r" (dst), [a] "r" (a), [b] "r" (b),
              [k1] "m" (k1), [k2] "m" (k2)
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void mix3(float *a, const float *b, const float *c, float k1, float k2, float k3, size_t count)
    {
        IF_ARCH_X86(size_t off);
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       %[k1], %%xmm5")
            __ASM_EMIT("movss       %[k2], %%xmm6")
            __ASM_EMIT("movss       %[k3], %%xmm7")
            __ASM_EMIT("xor         %[off], %[off]")
            __ASM_EMIT("shufps      $0x00, %%xmm5, %%xmm5")
            __ASM_EMIT("shufps      $0x00, %%xmm6, %%xmm6")
            __ASM_EMIT("shufps      $0x00, %%xmm7, %%xmm7")
            // 8x blocks
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jb          2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm5, %%xmm0")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm5, %%xmm1")
            __ASM_EMIT("mulps       %%xmm6, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[c],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[a],%[off])")
            __ASM_EMIT("add         $0x20, %[off]")
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jae         1b")
            // 4x block
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          4f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("mulps       %%xmm5, %%xmm0")
            __ASM_EMIT("mulps       %%xmm6, %%xmm1")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("addps       %%xmm1, %%xmm0")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("add         $0x10, %[off]")
            __ASM_EMIT("sub         $4, %[count]")
            // 1x blocks
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $3, %[count]")
            __ASM_EMIT("jl          6f")
            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movss       0x00(%[b],%[off]), %%xmm1")
            __ASM_EMIT("movss       0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("mulss       %%xmm5, %%xmm0")
            __ASM_EMIT("mulss       %%xmm6, %%xmm1")
            __ASM_EMIT("mulss       %%xmm7, %%xmm2")
            __ASM_EMIT("addss       %%xmm1, %%xmm0")
            __ASM_EMIT("addss       %%xmm2, %%xmm0")
            __ASM_EMIT("movss       %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("add         $0x04, %[off]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jge         5b")
            // End
            __ASM_EMIT("6:")
            : [count] "+r" (count),
              [off] "=&r" (off)
            : [a] "r" (a), [b] "r" (b), [c] "r" (c),
              [k1] "m" (k1), [k2] "m" (k2), [k3] "m" (k3)
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void mix_copy3(float *dst, const float *a, const float *b, const float *c, float k1, float k2, float k3, size_t count)
    {
        IF_ARCH_X86(size_t off);
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       %[k1], %%xmm5")
            __ASM_EMIT("movss       %[k2], %%xmm6")
            __ASM_EMIT("movss       %[k3], %%xmm7")
            __ASM_EMIT("xor         %[off], %[off]")
            __ASM_EMIT("shufps      $0x00, %%xmm5, %%xmm5")
            __ASM_EMIT("shufps      $0x00, %%xmm6, %%xmm6")
            __ASM_EMIT("shufps      $0x00, %%xmm7, %%xmm7")
            // 8x blocks
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jb          2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm5, %%xmm0")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm5, %%xmm1")
            __ASM_EMIT("mulps       %%xmm6, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[c],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst],%[off])")
            __ASM_EMIT("add         $0x20, %[off]")
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jae         1b")
            // 4x block
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          4f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("mulps       %%xmm5, %%xmm0")
            __ASM_EMIT("mulps       %%xmm6, %%xmm1")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("addps       %%xmm1, %%xmm0")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x10, %[off]")
            __ASM_EMIT("sub         $4, %[count]")
            // 1x blocks
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $3, %[count]")
            __ASM_EMIT("jl          6f")
            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movss       0x00(%[b],%[off]), %%xmm1")
            __ASM_EMIT("movss       0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("mulss       %%xmm5, %%xmm0")
            __ASM_EMIT("mulss       %%xmm6, %%xmm1")
            __ASM_EMIT("mulss       %%xmm7, %%xmm2")
            __ASM_EMIT("addss       %%xmm1, %%xmm0")
            __ASM_EMIT("addss       %%xmm2, %%xmm0")
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x04, %[off]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jge         5b")
            // End
            __ASM_EMIT("6:")
            : [count] "+r" (count),
              [off] "=&r" (off)
            : [dst] "r" (dst), [a] "r" (a), [b] "r" (b), [c] "r" (c),
              [k1] "m" (k1), [k2] "m" (k2), [k3] "m" (k3)
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void mix_add3(float *dst, const float *a, const float *b, const float *c, float k1, float k2, float k3, size_t count)
    {
        IF_ARCH_X86(size_t off);
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       %[k1], %%xmm5")
            __ASM_EMIT("movss       %[k2], %%xmm6")
            __ASM_EMIT("movss       %[k3], %%xmm7")
            __ASM_EMIT("xor         %[off], %[off]")
            __ASM_EMIT("shufps      $0x00, %%xmm5, %%xmm5")
            __ASM_EMIT("shufps      $0x00, %%xmm6, %%xmm6")
            __ASM_EMIT("shufps      $0x00, %%xmm7, %%xmm7")
            // 8x blocks
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jb          2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[dst],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[dst],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm5, %%xmm2")
            __ASM_EMIT("mulps       %%xmm5, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm6, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[c],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst],%[off])")
            __ASM_EMIT("add         $0x20, %[off]")
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jae         1b")
            // 4x block
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          4f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[dst],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm5, %%xmm1")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm1")
            __ASM_EMIT("addps       %%xmm3, %%xmm0")
            __ASM_EMIT("addps       %%xmm1, %%xmm0")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x10, %[off]")
            __ASM_EMIT("sub         $4, %[count]")
            // 1x blocks
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $3, %[count]")
            __ASM_EMIT("jl          6f")
            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[dst],%[off]), %%xmm0")
            __ASM_EMIT("movss       0x00(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movss       0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movss       0x00(%[c],%[off]), %%xmm3")
            __ASM_EMIT("mulss       %%xmm5, %%xmm1")
            __ASM_EMIT("mulss       %%xmm6, %%xmm2")
            __ASM_EMIT("mulss       %%xmm7, %%xmm3")
            __ASM_EMIT("addss       %%xmm2, %%xmm1")
            __ASM_EMIT("addss       %%xmm3, %%xmm0")
            __ASM_EMIT("addss       %%xmm1, %%xmm0")
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x04, %[off]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jge         5b")
            // End
            __ASM_EMIT("6:")
            : [count] "+r" (count),
              [off] "=&r" (off)
            : [dst] "r" (dst), [a] "r" (a), [b] "r" (b), [c] "r" (c),
              [k1] "m" (k1), [k2] "m" (k2), [k3] "m" (k3)
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void mix4(float *a, const float *b, const float *c, const float *d,
            float k1, float k2, float k3, float k4, size_t count)
    {
        IF_ARCH_X86(size_t off);
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       %[k1], %%xmm4")
            __ASM_EMIT("movss       %[k2], %%xmm5")
            __ASM_EMIT("movss       %[k3], %%xmm6")
            __ASM_EMIT("movss       %[k4], %%xmm7")
            __ASM_EMIT("xor         %[off], %[off]")
            __ASM_EMIT("shufps      $0x00, %%xmm4, %%xmm4")
            __ASM_EMIT("shufps      $0x00, %%xmm5, %%xmm5")
            __ASM_EMIT("shufps      $0x00, %%xmm6, %%xmm6")
            __ASM_EMIT("shufps      $0x00, %%xmm7, %%xmm7")
            // 8x blocks
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jb          2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm4, %%xmm0")
            __ASM_EMIT("mulps       %%xmm4, %%xmm1")
            __ASM_EMIT("mulps       %%xmm5, %%xmm2")
            __ASM_EMIT("mulps       %%xmm5, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[c],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm6, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[d],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[d],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[a],%[off])")
            __ASM_EMIT("add         $0x20, %[off]")
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jae         1b")
            // 4x block
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          4f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x00(%[d],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm4, %%xmm0")
            __ASM_EMIT("mulps       %%xmm5, %%xmm1")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm1, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm2")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("add         $0x10, %[off]")
            __ASM_EMIT("sub         $4, %[count]")
            // 1x blocks
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $3, %[count]")
            __ASM_EMIT("jl          6f")
            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movss       0x00(%[b],%[off]), %%xmm1")
            __ASM_EMIT("movss       0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movss       0x00(%[d],%[off]), %%xmm3")
            __ASM_EMIT("mulss       %%xmm4, %%xmm0")
            __ASM_EMIT("mulss       %%xmm5, %%xmm1")
            __ASM_EMIT("mulss       %%xmm6, %%xmm2")
            __ASM_EMIT("mulss       %%xmm7, %%xmm3")
            __ASM_EMIT("addss       %%xmm1, %%xmm0")
            __ASM_EMIT("addss       %%xmm3, %%xmm2")
            __ASM_EMIT("addss       %%xmm2, %%xmm0")
            __ASM_EMIT("movss       %%xmm0, 0x00(%[a],%[off])")
            __ASM_EMIT("add         $0x04, %[off]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jge         5b")
            // End
            __ASM_EMIT("6:")
            : [count] "+r" (count),
              [off] "=&r" (off)
            : [a] "r" (a), [b] "r" (b), [c] "r" (c), [d] "r" (d),
              [k1] "m" (k1), [k2] "m" (k2), [k3] "m" (k3), [k4] "m" (k4)
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void mix_copy4(float *dst, const float *a, const float *b, const float *c,
            const float *d, float k1, float k2, float k3, float k4, size_t count)
    {
        IF_ARCH_X86(size_t off);
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       %[k1], %%xmm4")
            __ASM_EMIT("movss       %[k2], %%xmm5")
            __ASM_EMIT("movss       %[k3], %%xmm6")
            __ASM_EMIT("movss       %[k4], %%xmm7")
            __ASM_EMIT("xor         %[off], %[off]")
            __ASM_EMIT("shufps      $0x00, %%xmm4, %%xmm4")
            __ASM_EMIT("shufps      $0x00, %%xmm5, %%xmm5")
            __ASM_EMIT("shufps      $0x00, %%xmm6, %%xmm6")
            __ASM_EMIT("shufps      $0x00, %%xmm7, %%xmm7")
            // 8x blocks
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jb          2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm4, %%xmm0")
            __ASM_EMIT("mulps       %%xmm4, %%xmm1")
            __ASM_EMIT("mulps       %%xmm5, %%xmm2")
            __ASM_EMIT("mulps       %%xmm5, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[c],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm6, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[d],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[d],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst],%[off])")
            __ASM_EMIT("add         $0x20, %[off]")
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jae         1b")
            // 4x block
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          4f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x00(%[d],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm4, %%xmm0")
            __ASM_EMIT("mulps       %%xmm5, %%xmm1")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm1, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm2")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x10, %[off]")
            __ASM_EMIT("sub         $4, %[count]")
            // 1x blocks
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $3, %[count]")
            __ASM_EMIT("jl          6f")
            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[a],%[off]), %%xmm0")
            __ASM_EMIT("movss       0x00(%[b],%[off]), %%xmm1")
            __ASM_EMIT("movss       0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movss       0x00(%[d],%[off]), %%xmm3")
            __ASM_EMIT("mulss       %%xmm4, %%xmm0")
            __ASM_EMIT("mulss       %%xmm5, %%xmm1")
            __ASM_EMIT("mulss       %%xmm6, %%xmm2")
            __ASM_EMIT("mulss       %%xmm7, %%xmm3")
            __ASM_EMIT("addss       %%xmm1, %%xmm0")
            __ASM_EMIT("addss       %%xmm3, %%xmm2")
            __ASM_EMIT("addss       %%xmm2, %%xmm0")
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x04, %[off]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jge         5b")
            // End
            __ASM_EMIT("6:")
            : [count] "+r" (count),
              [off] "=&r" (off)
            : [dst] "r" (dst), [a] "r" (a), [b] "r" (b), [c] "r" (c), [d] "r" (d),
              [k1] "m" (k1), [k2] "m" (k2), [k3] "m" (k3), [k4] "m" (k4)
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }

    void mix_add4(float *dst, const float *a, const float *b, const float *c,
            const float *d, float k1, float k2, float k3, float k4, size_t count)
    {
        IF_ARCH_X86(size_t off);
        ARCH_X86_ASM
        (
            __ASM_EMIT("movss       %[k1], %%xmm4")
            __ASM_EMIT("movss       %[k2], %%xmm5")
            __ASM_EMIT("movss       %[k3], %%xmm6")
            __ASM_EMIT("movss       %[k4], %%xmm7")
            __ASM_EMIT("xor         %[off], %[off]")
            __ASM_EMIT("shufps      $0x00, %%xmm4, %%xmm4")
            __ASM_EMIT("shufps      $0x00, %%xmm5, %%xmm5")
            __ASM_EMIT("shufps      $0x00, %%xmm6, %%xmm6")
            __ASM_EMIT("shufps      $0x00, %%xmm7, %%xmm7")
            // 8x blocks
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jb          2f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[dst],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x10(%[dst],%[off]), %%xmm1")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[a],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm4, %%xmm2")
            __ASM_EMIT("mulps       %%xmm4, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm5, %%xmm2")
            __ASM_EMIT("mulps       %%xmm5, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[c],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm6, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      0x00(%[d],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x10(%[d],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm7, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm1")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("movups      %%xmm1, 0x10(%[dst],%[off])")
            __ASM_EMIT("add         $0x20, %[off]")
            __ASM_EMIT("sub         $8, %[count]")
            __ASM_EMIT("jae         1b")
            // 4x block
            __ASM_EMIT("2:")
            __ASM_EMIT("add         $4, %[count]")
            __ASM_EMIT("jl          4f")
            __ASM_EMIT("1:")
            __ASM_EMIT("movups      0x00(%[dst],%[off]), %%xmm0")
            __ASM_EMIT("movups      0x00(%[a],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm4, %%xmm2")
            __ASM_EMIT("mulps       %%xmm5, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm0")
            __ASM_EMIT("movups      0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movups      0x00(%[d],%[off]), %%xmm3")
            __ASM_EMIT("mulps       %%xmm6, %%xmm2")
            __ASM_EMIT("mulps       %%xmm7, %%xmm3")
            __ASM_EMIT("addps       %%xmm2, %%xmm0")
            __ASM_EMIT("addps       %%xmm3, %%xmm0")
            __ASM_EMIT("movups      %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x10, %[off]")
            __ASM_EMIT("sub         $4, %[count]")
            // 1x blocks
            __ASM_EMIT("4:")
            __ASM_EMIT("add         $3, %[count]")
            __ASM_EMIT("jl          6f")
            __ASM_EMIT("5:")
            __ASM_EMIT("movss       0x00(%[dst],%[off]), %%xmm0")
            __ASM_EMIT("movss       0x00(%[a],%[off]), %%xmm2")
            __ASM_EMIT("movss       0x00(%[b],%[off]), %%xmm3")
            __ASM_EMIT("mulss       %%xmm4, %%xmm2")
            __ASM_EMIT("mulss       %%xmm5, %%xmm3")
            __ASM_EMIT("addss       %%xmm2, %%xmm0")
            __ASM_EMIT("addss       %%xmm3, %%xmm0")
            __ASM_EMIT("movss       0x00(%[c],%[off]), %%xmm2")
            __ASM_EMIT("movss       0x00(%[d],%[off]), %%xmm3")
            __ASM_EMIT("mulss       %%xmm6, %%xmm2")
            __ASM_EMIT("mulss       %%xmm7, %%xmm3")
            __ASM_EMIT("addss       %%xmm2, %%xmm0")
            __ASM_EMIT("addss       %%xmm3, %%xmm0")
            __ASM_EMIT("movss       %%xmm0, 0x00(%[dst],%[off])")
            __ASM_EMIT("add         $0x04, %[off]")
            __ASM_EMIT("dec         %[count]")
            __ASM_EMIT("jge         5b")
            // End
            __ASM_EMIT("6:")
            : [count] "+r" (count),
              [off] "=&r" (off)
            : [dst] "r" (dst), [a] "r" (a), [b] "r" (b), [c] "r" (c), [d] "r" (d),
              [k1] "m" (k1), [k2] "m" (k2), [k3] "m" (k3), [k4] "m" (k4)
            : "%xmm0", "%xmm1", "%xmm2", "%xmm3",
              "%xmm4", "%xmm5", "%xmm6", "%xmm7"
        );
    }
}

#endif /* DSP_ARCH_X86_SSE_MIX_H_ */
