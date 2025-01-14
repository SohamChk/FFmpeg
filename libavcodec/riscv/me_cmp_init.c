/*
 * Copyright (c) 2024 Institue of Software Chinese Academy of Sciences (ISCAS).
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "config.h"

#include "libavutil/attributes.h"
#include "libavutil/cpu.h"
#include "libavutil/riscv/cpu.h"
#include "libavcodec/me_cmp.h"
#include "libavcodec/mpegvideo.h"

int ff_pix_abs16_rvv(MpegEncContext *v, const uint8_t *pix1, const uint8_t *pix2,
                              ptrdiff_t stride, int h);
int ff_pix_abs8_rvv(MpegEncContext *v, const uint8_t *pix1, const uint8_t *pix2,
                             ptrdiff_t stride, int h);
int ff_pix_abs16_x2_rvv(MpegEncContext *v, const uint8_t *pix1, const uint8_t *pix2,
                         ptrdiff_t stride, int h);
int ff_pix_abs8_x2_rvv(MpegEncContext *v, const uint8_t *pix1, const uint8_t *pix2,
                         ptrdiff_t stride, int h);
int ff_pix_abs16_y2_rvv(MpegEncContext *v, const uint8_t *pix1, const uint8_t *pix2,
                          ptrdiff_t stride, int h);
int ff_pix_abs8_y2_rvv(MpegEncContext *v, const uint8_t *pix1, const uint8_t *pix2,
                          ptrdiff_t stride, int h);

int ff_sse16_rvv(MpegEncContext *v, const uint8_t *pix1, const uint8_t *pix2,
                   ptrdiff_t stride, int h);
int ff_sse8_rvv(MpegEncContext *v, const uint8_t *pix1, const uint8_t *pix2,
                   ptrdiff_t stride, int h);
int ff_sse4_rvv(MpegEncContext *v, const uint8_t *pix1, const uint8_t *pix2,
                   ptrdiff_t stride, int h);

int ff_vsse16_rvv(MpegEncContext *c, const uint8_t *s1, const uint8_t *s2, ptrdiff_t stride, int h);
int ff_vsse8_rvv(MpegEncContext *c, const uint8_t *s1, const uint8_t *s2, ptrdiff_t stride, int h);
int ff_vsse_intra16_rvv(MpegEncContext *c, const uint8_t *s, const uint8_t *dummy, ptrdiff_t stride, int h);
int ff_vsse_intra8_rvv(MpegEncContext *c, const uint8_t *s, const uint8_t *dummy, ptrdiff_t stride, int h);
int ff_vsad16_rvv(MpegEncContext *c, const uint8_t *s1, const uint8_t *s2, ptrdiff_t stride, int h);
int ff_vsad8_rvv(MpegEncContext *c, const uint8_t *s1, const uint8_t *s2, ptrdiff_t stride, int h);
int ff_vsad_intra16_rvv(MpegEncContext *c, const uint8_t *s, const uint8_t *dummy, ptrdiff_t stride, int h);
int ff_vsad_intra8_rvv(MpegEncContext *c, const uint8_t *s, const uint8_t *dummy, ptrdiff_t stride, int h);

av_cold void ff_me_cmp_init_riscv(MECmpContext *c, AVCodecContext *avctx)
{
#if HAVE_RVV
    int flags = av_get_cpu_flags();

    if (flags & AV_CPU_FLAG_RVV_I32 && ff_get_rv_vlenb() >= 16) {
        c->pix_abs[0][0] = ff_pix_abs16_rvv;
        c->sad[0] = ff_pix_abs16_rvv;
        c->pix_abs[1][0] = ff_pix_abs8_rvv;
        c->sad[1] = ff_pix_abs8_rvv;
        c->pix_abs[0][1] = ff_pix_abs16_x2_rvv;
        c->pix_abs[1][1] = ff_pix_abs8_x2_rvv;
        c->pix_abs[0][2] = ff_pix_abs16_y2_rvv;
        c->pix_abs[1][2] = ff_pix_abs8_y2_rvv;

        c->sse[0] = ff_sse16_rvv;
        c->sse[1] = ff_sse8_rvv;
        c->sse[2] = ff_sse4_rvv;

        c->vsse[0] = ff_vsse16_rvv;
        c->vsse[1] = ff_vsse8_rvv;
        c->vsse[4] = ff_vsse_intra16_rvv;
        c->vsse[5] = ff_vsse_intra8_rvv;
        c->vsad[0] = ff_vsad16_rvv;
        c->vsad[1] = ff_vsad8_rvv;
        c->vsad[4] = ff_vsad_intra16_rvv;
        c->vsad[5] = ff_vsad_intra8_rvv;
    }
#endif
}
