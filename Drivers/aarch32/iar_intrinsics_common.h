/**************************************************
*
* This file declares ARM intrinsic functions that are 
* common to IAR's intrinsics.h and arm_acle.h
*
* Copyright 2017-2018 IAR Systems. All rights reserved.
*
* $Revision$
*
**************************************************/
#ifndef _IAR_COMMON_ACLE_INTRINSICS_H
#define _IAR_COMMON_ACLE_INTRINSICS_H

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/* ACLE: Reads a system register */
#define __arm_rsr __iar_builtin_rsr
#define __arm_rsr64 __iar_builtin_rsr64
#define __arm_rsrp __iar_builtin_rsrp

/* ACLE: Writes a system register */
#define __arm_wsr __iar_builtin_wsr
#define __arm_wsr64 __iar_builtin_wsr64
#define __arm_wsrp __iar_builtin_wsrp

/* ACLE: Co-processor intrinsics */
#ifdef __ARM_FEATURE_COPROC
  #if __ARM_FEATURE_COPROC & 0x1
    #define __arm_cdp   __iar_builtin_CDP
    #define __arm_ldc   __iar_builtin_LDC
    #define __arm_ldcl  __iar_builtin_LDCL
    #define __arm_stc   __iar_builtin_STC
    #define __arm_stcl  __iar_builtin_STCL
    #define __arm_mcr   __iar_builtin_MCR
    #define __arm_mrc   __iar_builtin_MRC
  #endif
  #if __ARM_FEATURE_COPROC & 0x2
    #define __arm_cdp2  __iar_builtin_CDP2
    #define __arm_ldc2  __iar_builtin_LDC2
    #define __arm_ldc2l __iar_builtin_LDC2L
    #define __arm_stc2  __iar_builtin_STC2
    #define __arm_stc2l __iar_builtin_STC2L
    #define __arm_mcr2  __iar_builtin_MCR2
    #define __arm_mrc2  __iar_builtin_MRC2
  #endif
  #if __ARM_FEATURE_COPROC & 0x4
    #define __arm_mcrr __iar_builtin_MCRR
    #define __arm_mrrc __iar_builtin_MRRC
  #endif
  #if __ARM_FEATURE_COPROC & 0x8
    #define __arm_mcrr2 __iar_builtin_MCRR2
    #define __arm_mrrc2 __iar_builtin_MRRC2
  #endif
#endif

/* ACLE:  Floating-point data-processing intrinsics */
#ifdef __ARM_FP

  /* ACLE: square root */
  #if (__ARM_FP & 0x8)
    #define __sqrt      __iar_builtin_VSQRT_F64
  #endif
  #if (__ARM_FP & 0x4)
    #define  __sqrtf   __iar_builtin_VSQRT_F32
  #endif

  /* ACLE: fused mac */
  #ifdef __ARM_FEATURE_FMA
    #if (__ARM_FP & 0x8)
      #define __fma    __iar_builtin_VFMA_F64
    #endif
    #if (__ARM_FP & 0x4)
      #define __fmaf    __iar_builtin_VFMA_F32
    #endif
  #endif

  /* ACLE: directed rounding */
  #ifdef __ARM_FEATURE_DIRECTED_ROUNDING
    #define __rintn  __iar_builtin_VRINTN_F64
    #define __rintnf __iar_builtin_VRINTN_F32
  #endif

#endif


/* ACLE: CRC32 extension */
#ifdef __ARM_FEATURE_CRC32
  #define __crc32b(x,y)  __iar_builtin_CRC32B((x), (y))
  #define __crc32h(x,y)  __iar_builtin_CRC32H((x), (y))
  #define __crc32w(x,y)  __iar_builtin_CRC32W((x), (y))
  #define __crc32cb(x,y) __iar_builtin_CRC32CB((x), (y))
  #define __crc32ch(x,y) __iar_builtin_CRC32CH((x), (y))
  #define __crc32cw(x,y) __iar_builtin_CRC32CW((x), (y))

  #ifdef __aarch64__
    #define __crc32d(x,y)  __iar_builtin_CRC32X((x), (y))
    #define __crc32cd(x,y) __iar_builtin_CRC32CX((x), (y))
  #else
    #pragma inline=forced
    unsigned int __crc32d(unsigned int crc, unsigned long long data)
    {
      crc = __crc32w(crc, (unsigned int)(data));
      crc = __crc32w(crc, (unsigned int)(data >> 32));
      return crc;
    }
    #pragma inline=forced
    unsigned int __crc32cd(unsigned int crc, unsigned long long data)
    {
      crc = __crc32cw(crc, (unsigned int)(data));
      crc = __crc32cw(crc, (unsigned int)(data >> 32));
      return crc;
    }
  #endif
#endif


#endif /* _IAR_COMMON_ACLE_INTRINSICS_H */
