/*
 * rcp.cpp
 *
 *  Created on: 23 авг. 2018 г.
 *      Author: sadko
 */

#include <dsp/dsp.h>
#include <test/utest.h>
#include <test/FloatBuffer.h>

namespace native
{
    void complex_rcp1(float *dst_re, float *dst_im, size_t count);
    void complex_rcp2(float *dst_re, float *dst_im, const float *src_re, const float *src_im, size_t count);
    void pcomplex_rcp1(float *dst, size_t count);
    void pcomplex_rcp2(float *dst, const float *src, size_t count);
}

IF_ARCH_X86(
    namespace sse
    {
        void complex_rcp1(float *dst_re, float *dst_im, size_t count);
        void complex_rcp2(float *dst_re, float *dst_im, const float *src_re, const float *src_im, size_t count);
        void pcomplex_rcp1(float *dst, size_t count);
        void pcomplex_rcp2(float *dst, const float *src, size_t count);
    }
)

typedef void (* complex_rcp1_t) (float *dst_re, float *dst_im, size_t count);
typedef void (* complex_rcp2_t) (float *dst_re, float *dst_im, const float *src_re, const float *src_im, size_t count);
typedef void (* packed_complex_rcp1_t) (float *dst, size_t count);
typedef void (* packed_complex_rcp2_t) (float *dst, const float *src, size_t count);


UTEST_BEGIN("dsp.complex", rcp)

    void call(const char *text, size_t align, complex_rcp1_t func)
    {
        if (!UTEST_SUPPORTED(func))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                32, 64, 65, 100, 768, 999, 1024)
        {
            for (size_t mask=0; mask <= 0x03; ++mask)
            {
                printf("Testing %s on input buffer of %d numbers, mask=0x%x...\n", text, int(count), int(mask));

                FloatBuffer dst1_re(count, align, mask & 0x01);
                FloatBuffer dst1_im(count, align, mask & 0x02);
                FloatBuffer dst2_re(dst1_re);
                FloatBuffer dst2_im(dst1_im);

                // Call functions
                native::complex_rcp1(dst1_re, dst1_im, count);
                func(dst2_re, dst2_im, count);

                UTEST_ASSERT_MSG(dst1_re.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst1_im.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2_re.valid(), "Destination buffer 2 corrupted");
                UTEST_ASSERT_MSG(dst2_im.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if ((!dst1_re.equals_absolute(dst2_re, 1e-5)) || (!dst1_im.equals_absolute(dst2_im, 1e-5)))
                {
                    dst1_re.dump("dst1_re");
                    dst1_im.dump("dst1_im");
                    dst2_re.dump("dst2_re");
                    dst2_im.dump("dst2_im");
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", text);
                }
            }
        }
    }

    void call(const char *text, size_t align, complex_rcp2_t func)
    {
        if (!UTEST_SUPPORTED(func))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                32, 64, 65, 100, 768, 999, 1024)
        {
            for (size_t mask=0; mask <= 0x0f; ++mask)
            {
                printf("Testing %s on input buffer of %d numbers, mask=0x%x...\n", text, int(count), int(mask));

                FloatBuffer src_re(count, align, mask & 0x01);
                FloatBuffer src_im(count, align, mask & 0x02);
                FloatBuffer dst1_re(count, align, mask & 0x04);
                FloatBuffer dst1_im(count, align, mask & 0x08);
                FloatBuffer dst2_re(dst1_re);
                FloatBuffer dst2_im(dst1_im);

                // Call functions
                native::complex_rcp2(dst1_re, dst1_im, src_re, src_im, count);
                func(dst2_re, dst2_im, src_re, src_im, count);

                UTEST_ASSERT_MSG(src_re.valid(), "Source buffer corrupted");
                UTEST_ASSERT_MSG(src_im.valid(), "Source buffer corrupted");
                UTEST_ASSERT_MSG(dst1_re.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst1_im.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2_re.valid(), "Destination buffer 2 corrupted");
                UTEST_ASSERT_MSG(dst2_im.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if ((!dst1_re.equals_absolute(dst2_re, 1e-5)) || (!dst1_im.equals_absolute(dst2_im, 1e-5)))
                {
                    src_re.dump("src_re");
                    src_im.dump("src_im");
                    dst1_re.dump("dst1_re");
                    dst1_im.dump("dst1_im");
                    dst2_re.dump("dst2_re");
                    dst2_im.dump("dst2_im");
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", text);
                }
            }
        }
    }

    void call(const char *text, size_t align, packed_complex_rcp1_t func)
    {
        if (!UTEST_SUPPORTED(func))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                32, 64, 65, 100, 768, 999, 1024)
        {
            for (size_t mask=0; mask <= 0x01; ++mask)
            {
                printf("Testing %s on input buffer of %d numbers, mask=0x%x...\n", text, int(count), int(mask));

                FloatBuffer dst1(count*2, align, mask & 0x01);
                FloatBuffer dst2(dst1);

                // Call functions
                native::pcomplex_rcp1(dst1, count);
                func(dst2, count);

                UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if (!dst1.equals_absolute(dst2, 1e-5))
                {
                    dst1.dump("dst1");
                    dst2.dump("dst2");
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", text);
                }
            }
        }
    }

    void call(const char *text, size_t align, packed_complex_rcp2_t func)
    {
        if (!UTEST_SUPPORTED(func))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                32, 64, 65, 100, 768, 999, 1024)
        {
            for (size_t mask=0; mask <= 0x03; ++mask)
            {
                printf("Testing %s on input buffer of %d numbers, mask=0x%x...\n", text, int(count), int(mask));

                FloatBuffer src(count*2, align, mask & 0x01);
                FloatBuffer dst1(count*2, align, mask & 0x02);
                FloatBuffer dst2(dst1);

                // Call functions
                native::pcomplex_rcp2(dst1, src, count);
                func(dst2, src, count);

                UTEST_ASSERT_MSG(src.valid(), "Source buffer corrupted");
                UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if (!dst1.equals_absolute(dst2, 1e-5))
                {
                    src.dump("dst");
                    dst1.dump("dst1");
                    dst2.dump("dst2");
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", text);
                }
            }
        }
    }

    UTEST_MAIN
    {
        IF_ARCH_X86(call("complex_rcp1_sse", 16, sse::complex_rcp1));
        IF_ARCH_X86(call("complex_rcp2_sse", 16, sse::complex_rcp2));
        IF_ARCH_X86(call("packed_complex_rcp1_sse", 16, sse::pcomplex_rcp1));
        IF_ARCH_X86(call("packed_complex_rcp2_sse", 16, sse::pcomplex_rcp2));
    }

UTEST_END;
