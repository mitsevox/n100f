#include <rwcore.h>
#include <dolphin/gx/GXManage.h>

// The draw sync token last issued to the GPU, and the one last read back.
extern U16 lbl_8025E800;
extern U16 lbl_8025F890;

// Returns TRUE once the GPU has passed the given draw sync token. Tokens wrap around, so the
// test depends on whether the issued token has wrapped past the one last read.
S32 _rwDlTokenQueryDone(U16 token)
{
    S32 done;

    lbl_8025F890 = GXReadDrawSync();
    if (lbl_8025E800 >= lbl_8025F890)
    {
        done = FALSE;
        if ((U16)token <= lbl_8025F890 || (U16)token > lbl_8025E800)
        {
            done = TRUE;
        }
    }
    else
    {
        done = FALSE;
        if ((U16)token > lbl_8025E800 && (U16)token <= lbl_8025F890)
        {
            done = TRUE;
        }
    }
    return done;
}

// The paired-single matrix multiply _rwDlSystem installs with _rwMatrixSetMultFn:
// dst = a * b, for the three axes and the position. The fourth word of each row is not
// written.
asm void DlMatrixMultiplyPaired(register RwMatrix* dst, register const RwMatrix* a,
                     register const RwMatrix* b)
{
    nofralloc
    stwu r1, -0x58(r1)
    stfd f14, 0x8(r1)
    stfd f15, 0x10(r1)
    stfd f16, 0x18(r1)
    stfd f17, 0x20(r1)
    stfd f18, 0x28(r1)
    stfd f19, 0x30(r1)
    stfd f20, 0x38(r1)
    stfd f21, 0x40(r1)
    stfd f22, 0x48(r1)
    stfd f23, 0x50(r1)
    psq_l f0, 0x0(a), 0, 0
    psq_l f1, 0x8(a), 1, 0
    psq_l f2, 0x10(a), 0, 0
    psq_l f3, 0x18(a), 1, 0
    psq_l f4, 0x20(a), 0, 0
    psq_l f5, 0x28(a), 1, 0
    psq_l f6, 0x30(a), 0, 0
    psq_l f7, 0x38(a), 1, 0
    psq_l f8, 0x0(b), 0, 0
    psq_l f9, 0x8(b), 1, 0
    psq_l f10, 0x10(b), 0, 0
    psq_l f11, 0x18(b), 1, 0
    psq_l f12, 0x20(b), 0, 0
    psq_l f13, 0x28(b), 1, 0
    psq_l f14, 0x30(b), 0, 0
    psq_l f15, 0x38(b), 1, 0
    ps_muls0 f16, f8, f0
    ps_muls0 f18, f8, f2
    ps_muls0 f20, f8, f4
    ps_madds0 f22, f8, f6, f14
    ps_madds1 f16, f10, f0, f16
    ps_madds1 f18, f10, f2, f18
    ps_madds1 f20, f10, f4, f20
    ps_madds1 f22, f10, f6, f22
    ps_madds0 f16, f12, f1, f16
    ps_madds0 f18, f12, f3, f18
    ps_madds0 f20, f12, f5, f20
    ps_madds0 f22, f12, f7, f22
    ps_muls0 f17, f9, f0
    ps_muls0 f19, f9, f2
    ps_muls0 f21, f9, f4
    ps_madds0 f23, f9, f6, f15
    ps_madds1 f17, f11, f0, f17
    ps_madds1 f19, f11, f2, f19
    ps_madds1 f21, f11, f4, f21
    ps_madds1 f23, f11, f6, f23
    ps_madds0 f17, f13, f1, f17
    ps_madds0 f19, f13, f3, f19
    ps_madds0 f21, f13, f5, f21
    ps_madds0 f23, f13, f7, f23
    psq_st f16, 0x0(dst), 0, 0
    psq_st f17, 0x8(dst), 1, 0
    psq_st f18, 0x10(dst), 0, 0
    psq_st f19, 0x18(dst), 1, 0
    psq_st f20, 0x20(dst), 0, 0
    psq_st f21, 0x28(dst), 1, 0
    psq_st f22, 0x30(dst), 0, 0
    psq_st f23, 0x38(dst), 1, 0
    lfd f14, 0x8(r1)
    lfd f15, 0x10(r1)
    lfd f16, 0x18(r1)
    lfd f17, 0x20(r1)
    lfd f18, 0x28(r1)
    lfd f19, 0x30(r1)
    lfd f20, 0x38(r1)
    lfd f21, 0x40(r1)
    lfd f22, 0x48(r1)
    lfd f23, 0x50(r1)
    addi r1, r1, 0x58
    blr
}

// The paired-single point transform _rwDlSystem installs with _rwVectorSetMultFn:
// each of count points is multiplied by the matrix's axes and offset by its position.
asm void DlV3dTransformPointsPaired(register RwV3d* dst, register const RwV3d* src, register S32 count,
                     register const RwMatrix* matrix)
{
    nofralloc
    cmpwi count, 0
    mtctr count
    subi r7, dst, 4
    subi src, src, 4
    blelr
    psq_l f0, 0x0(matrix), 0, 0
    psq_l f1, 0x8(matrix), 1, 0
    psq_l f2, 0x10(matrix), 0, 0
    psq_l f3, 0x18(matrix), 1, 0
    psq_l f4, 0x20(matrix), 0, 0
    psq_l f5, 0x28(matrix), 1, 0
    psq_l f6, 0x30(matrix), 0, 0
    psq_l f7, 0x38(matrix), 1, 0
    psq_lu f8, 0x4(src), 1, 0
    psq_lu f9, 0x4(src), 1, 0
    psq_lu f10, 0x4(src), 1, 0
@1:
    ps_madds0 f11, f0, f8, f6
    ps_madds0 f12, f1, f8, f7
    psq_lu f8, 0x4(src), 1, 0
    ps_madds0 f11, f2, f9, f11
    ps_madds0 f12, f3, f9, f12
    psq_lu f9, 0x4(src), 1, 0
    ps_madds0 f11, f4, f10, f11
    ps_madds0 f12, f5, f10, f12
    psq_lu f10, 0x4(src), 1, 0
    ps_merge11 f13, f11, f11
    psq_stu f11, 0x4(r7), 1, 0
    psq_stu f13, 0x4(r7), 1, 0
    psq_stu f12, 0x4(r7), 1, 0
    bdnz @1
    blr
}

// The vector transform installed beside it: the same, without the position.
asm void DlV3dTransformVectorsPaired(register RwV3d* dst, register const RwV3d* src, register S32 count,
                     register const RwMatrix* matrix)
{
    nofralloc
    cmpwi count, 0
    mtctr count
    subi r7, dst, 4
    subi src, src, 4
    blelr
    psq_l f0, 0x0(matrix), 0, 0
    psq_l f1, 0x8(matrix), 1, 0
    psq_l f2, 0x10(matrix), 0, 0
    psq_l f3, 0x18(matrix), 1, 0
    psq_l f4, 0x20(matrix), 0, 0
    psq_l f5, 0x28(matrix), 1, 0
    psq_l f6, 0x30(matrix), 0, 0
    psq_l f7, 0x38(matrix), 1, 0
    psq_lu f8, 0x4(src), 1, 0
    psq_lu f9, 0x4(src), 1, 0
    psq_lu f10, 0x4(src), 1, 0
@1:
    ps_muls0 f11, f0, f8
    ps_muls0 f12, f1, f8
    psq_lu f8, 0x4(src), 1, 0
    ps_madds0 f11, f2, f9, f11
    ps_madds0 f12, f3, f9, f12
    psq_lu f9, 0x4(src), 1, 0
    ps_madds0 f11, f4, f10, f11
    ps_madds0 f12, f5, f10, f12
    psq_lu f10, 0x4(src), 1, 0
    ps_merge11 f13, f11, f11
    psq_stu f11, 0x4(r7), 1, 0
    psq_stu f13, 0x4(r7), 1, 0
    psq_stu f12, 0x4(r7), 1, 0
    bdnz @1
    blr
}
