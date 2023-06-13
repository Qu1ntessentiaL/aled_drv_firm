/**************************************************
 *
 * This file declares the ARM intrinsic inline functions.
 *
 * Copyright 1999-2018 IAR Systems. All rights reserved.
 *
 * $Revision$
 *
 **************************************************/

#ifndef __INTRINSICS_INCLUDED
#define __INTRINSICS_INCLUDED

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/*
 * Check that the correct C compiler is used.
 */
#ifndef __ICCARM__
  #error "File intrinsics.h can only be used together with iccarm."
#endif

#ifndef __ICCARM_INTRINSICS_VERSION__
  #error "Unknown compiler intrinsics version"
#elif __ICCARM_INTRINSICS_VERSION__ != 2
  #error "Compiler intrinsics version does not match this file"
#endif

#include <iccarm_builtin.h>
#include <iar_intrinsics_common.h>

#define __no_operation __iar_builtin_no_operation
#define __disable_interrupt __iar_builtin_disable_interrupt
#define __enable_interrupt __iar_builtin_enable_interrupt
#define __get_interrupt_state __iar_builtin_get_interrupt_state
#define __set_interrupt_state __iar_builtin_set_interrupt_state

#ifdef __ARM_PROFILE_M__
  /* System control access for Cortex-M cores */
  #ifndef __get_PSR
    #define __get_PSR       __iar_builtin_get_PSR
  #endif
  #ifndef __get_IPSR
    #define __get_IPSR      __iar_builtin_get_IPSR
  #endif
  #ifndef __get_MSP
    #define __get_MSP       __iar_builtin_get_MSP
  #endif
  #ifndef __set_MSP
    #define __set_MSP       __iar_builtin_set_MSP
  #endif
  #ifndef __get_PSP
    #define __get_PSP       __iar_builtin_get_PSP
  #endif
  #ifndef __set_PSP
    #define __set_PSP       __iar_builtin_set_PSP
  #endif
  #ifndef __get_PRIMASK
    #define __get_PRIMASK   __iar_builtin_get_PRIMASK
  #endif
  #ifndef __set_PRIMASK
    #define __set_PRIMASK   __iar_builtin_set_PRIMASK
  #endif
  #ifndef __get_CONTROL
    #define __get_CONTROL   __iar_builtin_get_CONTROL
  #endif
  #ifndef __set_CONTROL
    #define __set_CONTROL   __iar_builtin_set_CONTROL
  #endif

  #ifdef __ARM7__
    /* These are only available for v7M */
    #ifndef __get_FAULTMASK
      #define __get_FAULTMASK __iar_builtin_get_FAULTMASK
    #endif
    #ifndef __set_FAULTMASK
      #define __set_FAULTMASK __iar_builtin_set_FAULTMASK
    #endif
    #ifndef __get_BASEPRI
      #define __get_BASEPRI __iar_builtin_get_BASEPRI
    #endif
    #ifndef __set_BASEPRI
      #define __set_BASEPRI __iar_builtin_set_BASEPRI
    #endif
  #endif /* __ARM7__ */

#else /* __ARM_PROFILE_M__ */

  /* "Old" style interrupt control routines */
  #ifndef __disable_irq
    #define __disable_irq __iar_builtin_disable_irq
  #endif
  #ifndef __enable_irq
    #define __enable_irq __iar_builtin_enable_irq
  #endif
#endif /* __ARM_PROFILE_M__ */

#ifndef __disable_fiq
  #define __disable_fiq __iar_builtin_disable_fiq
#endif
#ifndef __enable_fiq
  #define __enable_fiq __iar_builtin_enable_fiq
#endif

/* ARM-mode intrinsics */
#ifndef __SWP
  #define __SWP __iar_builtin_SWP
#endif
#ifndef __SWPB
  #define __SWPB __iar_builtin_SWPB
#endif

/*  Co-processor operations */
#ifndef __CDP
  #define __CDP __iar_builtin_CDP
#endif
#ifndef __CDP2
  #define __CDP2 __iar_builtin_CDP2
#endif

/*  Co-processor access */
#ifndef __MCR
  #define __MCR __iar_builtin_MCR
#endif
#ifndef __MRC
  #define __MRC __iar_builtin_MRC
#endif
#ifndef __MCR2
  #define __MCR2 __iar_builtin_MCR2
#endif
#ifndef __MRC2
  #define __MRC2 __iar_builtin_MRC2
#endif
#ifndef __MCRR
  #define __MCRR __iar_builtin_MCRR
#endif
#ifndef __MCRR2
  #define __MCRR2 __iar_builtin_MCRR2
#endif
#ifndef __MRRC
  #define __MRRC __iar_builtin_MRRC
#endif
#ifndef __MRRC2
  #define __MRRC2 __iar_builtin_MRRC2
#endif

/* Load coprocessor register. */
#ifndef __LDC
  #define __LDC __iar_builtin_LDC
#endif
#ifndef __LDCL
  #define __LDCL __iar_builtin_LDCL
#endif
#ifndef __LDC2
  #define __LDC2 __iar_builtin_LDC2
#endif
#ifndef __LDC2L
  #define __LDC2L __iar_builtin_LDC2L
#endif

/* Store coprocessor register. */
#ifndef __STC
  #define __STC __iar_builtin_STC
#endif
#ifndef __STCL
  #define __STCL __iar_builtin_STCL
#endif
#ifndef __STC2
  #define __STC2 __iar_builtin_STC2
#endif
#ifndef __STC2L
  #define __STC2L __iar_builtin_STC2L
#endif

/* Load coprocessor register (noindexed version with coprocessor option). */
#ifndef __LDC_noidx
  #define __LDC_noidx __iar_builtin_LDC_noidx
#endif
#ifndef __LDCL_noidx
  #define __LDCL_noidx __iar_builtin_LDCL_noidx
#endif
#ifndef __LDC2_noidx
  #define __LDC2_noidx __iar_builtin_LDC2_noidx
#endif
#ifndef __LDC2L_noidx
  #define __LDC2L_noidx __iar_builtin_LDC2L_noidx
#endif

/* Store coprocessor register (version with coprocessor option). */
#ifndef __STC_noidx
  #define __STC_noidx __iar_builtin_STC_noidx
#endif
#ifndef __STCL_noidx
  #define __STCL_noidx __iar_builtin_STCL_noidx
#endif
#ifndef __STC2_noidx
  #define __STC2_noidx __iar_builtin_STC2_noidx
#endif
#ifndef __STC2L_noidx
  #define __STC2L_noidx __iar_builtin_STC2L_noidx
#endif

/* square root */
#ifndef __VSQRT_F64
  #define __VSQRT_F64(x) __iar_builtin_VSQRT_F64(x)
#endif
#ifndef __VSQRT_F32
  #define __VSQRT_F32(x) __iar_builtin_VSQRT_F32(x)
#endif

/* fused mac */
#ifndef __VFMA_F64
  #define __VFMA_F64(a,x,y)  __iar_builtin_VFMA_F64((x), (y), (a))
#endif
#ifndef __VFMS_F64
  #define __VFMS_F64(a,x,y)  __iar_builtin_VFMS_F64((x), (y), (a))
#endif
#ifndef __VFNMA_F64
  #define __VFNMA_F64(a,x,y) __iar_builtin_VFNMA_F64((x), (y), (a))
#endif
#ifndef __VFNMS_F64
  #define __VFNMS_F64(a,x,y) __iar_builtin_VFNMS_F64((x), (y), (a))
#endif
#ifndef __VFMA_F32
  #define __VFMA_F32(a,x,y)  __iar_builtin_VFMA_F32((x), (y), (a))
#endif
#ifndef __VFMS_F32
  #define __VFMS_F32(a,x,y)  __iar_builtin_VFMS_F32((x), (y), (a))
#endif
#ifndef __VFNMA_F32
  #define __VFNMA_F32(a,x,y) __iar_builtin_VFNMA_F32((x), (y), (a))
#endif
#ifndef __VFNMS_F32
  #define __VFNMS_F32(a,x,y) __iar_builtin_VFNMS_F32((x), (y), (a))
#endif

/* directed rounding.
 * NOTE: for example VRINTA becomes VCVTA when converted to int.
 */
#ifndef __VRINTA_F64
  #define __VRINTA_F64(x) __iar_builtin_VRINTA_F64(x)
#endif
#ifndef __VRINTM_F64
  #define __VRINTM_F64(x) __iar_builtin_VRINTM_F64(x)
#endif
#ifndef __VRINTN_F64
  #define __VRINTN_F64(x) __iar_builtin_VRINTN_F64(x)
#endif
#ifndef __VRINTP_F64
  #define __VRINTP_F64(x) __iar_builtin_VRINTP_F64(x)
#endif
#ifndef __VRINTX_F64
  #define __VRINTX_F64(x) __iar_builtin_VRINTX_F64(x)
#endif
#ifndef __VRINTR_F64
  #define __VRINTR_F64(x) __iar_builtin_VRINTR_F64(x)
#endif
#ifndef __VRINTZ_F64
  #define __VRINTZ_F64(x) __iar_builtin_VRINTZ_F64(x)
#endif
#ifndef __VRINTA_F32
  #define __VRINTA_F32(x) __iar_builtin_VRINTA_F32(x)
#endif
#ifndef __VRINTM_F32
  #define __VRINTM_F32(x) __iar_builtin_VRINTM_F32(x)
#endif
#ifndef __VRINTN_F32
  #define __VRINTN_F32(x) __iar_builtin_VRINTN_F32(x)
#endif
#ifndef __VRINTP_F32
  #define __VRINTP_F32(x) __iar_builtin_VRINTP_F32(x)
#endif
#ifndef __VRINTX_F32
  #define __VRINTX_F32(x) __iar_builtin_VRINTX_F32(x)
#endif
#ifndef __VRINTR_F32
  #define __VRINTR_F32(x) __iar_builtin_VRINTR_F32(x)
#endif
#ifndef __VRINTZ_F32
  #define __VRINTZ_F32(x) __iar_builtin_VRINTZ_F32(x)
#endif

/* numeric min/max */
#ifndef __VMINNM_F64
  #define __VMINNM_F64(x,y) __iar_builtin_VMINNM_F64((x), (y))
#endif
#ifndef __VMAXNM_F64
  #define __VMAXNM_F64(x,y) __iar_builtin_VMAXNM_F64((x), (y))
#endif
#ifndef __VMINNM_F32
  #define __VMINNM_F32(x,y) __iar_builtin_VMINNM_F32((x), (y))
#endif
#ifndef __VMAXNM_F32
  #define __VMAXNM_F32(x,y) __iar_builtin_VMAXNM_F32((x), (y))
#endif

/* CRC32 extension */
#ifndef __CRC32B
  #define __CRC32B(x,y)   __iar_builtin_CRC32B((x), (y))
#endif
#ifndef __CRC32H
  #define __CRC32H(x,y)   __iar_builtin_CRC32H((x), (y))
#endif
#ifndef __CRC32W
  #define __CRC32W(x,y)   __iar_builtin_CRC32W((x), (y))
#endif
#ifndef __CRC32CB
  #define __CRC32CB(x,y)  __iar_builtin_CRC32CB((x), (y))
#endif
#ifndef __CRC32CH
  #define __CRC32CH(x,y)  __iar_builtin_CRC32CH((x), (y))
#endif
#ifndef __CRC32CW
  #define __CRC32CW(x,y)  __iar_builtin_CRC32CW((x), (y))
#endif

#ifdef __ARM_PROFILE_M__
  /* Status register access, v7M: */
  #ifndef __get_APSR
    #define __get_APSR    __iar_builtin_get_APSR
  #endif
  #ifndef __set_APSR
    #define __set_APSR    __iar_builtin_set_APSR
  #endif
#else /* __ARM_PROFILE_M__ */
  /* Status register access */
  #ifndef __get_CPSR
    #define __get_CPSR    __iar_builtin_get_CPSR
  #endif
  #ifndef __set_CPSR
    #define __set_CPSR    __iar_builtin_set_CPSR
  #endif
#endif /* __ARM_PROFILE_M__ */

/* Floating-point status and control register access */
#ifndef __get_FPSCR
  #define __get_FPSCR     __iar_builtin_get_FPSCR
#endif
#ifndef __set_FPSCR
  #define __set_FPSCR     __iar_builtin_set_FPSCR
#endif

/* Architecture v5T, CLZ is also available in Thumb mode for Thumb2 cores */
#ifndef __CLZ
  #define __CLZ __iar_builtin_CLZ
#endif
#ifndef __ROR
  #define __ROR           __iar_builtin_ROR
#endif
#ifndef __RRX
  #define __RRX           __iar_builtin_RRX
#endif

/* Architecture v5TE */
#if !defined(__ARM_PROFILE_M__) || defined(__ARM_MEDIA__)
  #ifndef __QADD
    #define __QADD        __iar_builtin_QADD
  #endif
  #ifndef __QDADD
    #define __QDADD       __iar_builtin_QDADD
  #endif
  #ifndef __QSUB
    #define __QSUB        __iar_builtin_QSUB
  #endif
  #ifndef __QDSUB
    #define __QDSUB       __iar_builtin_QDSUB
  #endif
  #ifndef __QDOUBLE
    #define __QDOUBLE     __iar_builtin_QDOUBLE
  #endif
  #ifndef __QFlag
    #define __QFlag          __iar_builtin_QFlag
  #endif
  #ifndef __reset_Q_flag
    #define __reset_Q_flag() __iar_builtin_set_QFlag(0)
  #endif
  #ifndef __set_Q_flag
    #define __set_Q_flag     __iar_builtin_set_QFlag
  #endif
#endif

#ifndef __QCFlag
  #define __QCFlag        __iar_builtin_QCFlag
#endif
#ifndef __reset_QC_flag
  #define __reset_QC_flag __iar_builtin_reset_QC_flag
#endif

#ifndef __SMUL
  #define __SMUL          __iar_builtin_SMUL
#endif

/* Architecture v6, REV and REVSH are also available in thumb mode */
#ifndef __REV
  #define __REV           __iar_builtin_REV
#endif
#ifndef __REVSH
  #define __REVSH       __iar_builtin_REVSH
#endif

#ifndef __REV16
  #define __REV16         __iar_builtin_REV16
#endif
#ifndef __RBIT
  #define __RBIT          __iar_builtin_RBIT
#endif

#ifndef __LDREXB
  #define __LDREXB        __iar_builtin_LDREXB
#endif
#ifndef __LDREXH
  #define __LDREXH        __iar_builtin_LDREXH
#endif
#ifndef __LDREX
  #define __LDREX         __iar_builtin_LDREX
#endif
#ifndef __LDREXD
  #define __LDREXD        __iar_builtin_LDREXD
#endif

#ifndef __STREXB
  #define __STREXB        __iar_builtin_STREXB
#endif
#ifndef __STREXH
  #define __STREXH        __iar_builtin_STREXH
#endif
#ifndef __STREX
  #define __STREX         __iar_builtin_STREX
#endif
#ifndef __STREXD
  #define __STREXD        __iar_builtin_STREXD
#endif

#ifndef __CLREX
  #define __CLREX         __iar_builtin_CLREX
#endif

#ifndef __SEV
  #define __SEV           __iar_builtin_SEV
#endif
#ifndef __WFE
  #define __WFE           __iar_builtin_WFE
#endif
#ifndef __WFI
  #define __WFI           __iar_builtin_WFI
#endif
#ifndef __YIELD
  #define __YIELD         __iar_builtin_YIELD
#endif

#ifndef __PLI
  #define __PLI           __iar_builtin_PLI
#endif
#ifndef __PLD
  #define __PLD           __iar_builtin_PLD
#endif
#ifndef __PLDW
  #define __PLDW          __iar_builtin_PLDW
#endif

#ifndef __SSAT
  #define __SSAT        __iar_builtin_SSAT
#endif
#ifndef __USAT
  #define __USAT        __iar_builtin_USAT
#endif

#if __ARM_MEDIA__
  /* Architecture v6 Media instructions */
  #ifndef __SEL
    #define __SEL         __iar_builtin_SEL
  #endif
  #ifndef __SADD8
    #define __SADD8       __iar_builtin_SADD8
  #endif
  #ifndef __SADD16
    #define __SADD16      __iar_builtin_SADD16
  #endif
  #ifndef __SSUB8
    #define __SSUB8       __iar_builtin_SSUB8
  #endif
  #ifndef __SSUB16
    #define __SSUB16      __iar_builtin_SSUB16
  #endif
  #ifndef __SADDSUBX
    #define __SADDSUBX    __iar_builtin_SADDSUBX
  #endif
  #ifndef __SSUBADDX
    #define __SSUBADDX    __iar_builtin_SSUBADDX
  #endif
  #ifndef __SHADD8
    #define __SHADD8      __iar_builtin_SHADD8
  #endif
  #ifndef __SHADD16
    #define __SHADD16     __iar_builtin_SHADD16
  #endif
  #ifndef __SHSUB8
    #define __SHSUB8      __iar_builtin_SHSUB8
  #endif
  #ifndef __SHSUB16
    #define __SHSUB16     __iar_builtin_SHSUB16
  #endif
  #ifndef __SHADDSUBX
    #define __SHADDSUBX   __iar_builtin_SHADDSUBX
  #endif
  #ifndef __SHSUBADDX
    #define __SHSUBADDX   __iar_builtin_SHSUBADDX
  #endif
  #ifndef __QADD8
    #define __QADD8       __iar_builtin_QADD8
  #endif
  #ifndef __QADD16
    #define __QADD16      __iar_builtin_QADD16
  #endif
  #ifndef __QSUB8
    #define __QSUB8       __iar_builtin_QSUB8
  #endif
  #ifndef __QSUB16
    #define __QSUB16      __iar_builtin_QSUB16
  #endif
  #ifndef __QADDSUBX
    #define __QADDSUBX    __iar_builtin_QADDSUBX
  #endif
  #ifndef __QSUBADDX
    #define __QSUBADDX    __iar_builtin_QSUBADDX
  #endif
  #ifndef __UADD8
    #define __UADD8       __iar_builtin_UADD8
  #endif
  #ifndef __UADD16
    #define __UADD16      __iar_builtin_UADD16
  #endif
  #ifndef __USUB8
    #define __USUB8       __iar_builtin_USUB8
  #endif
  #ifndef __USUB16
    #define __USUB16      __iar_builtin_USUB16
  #endif
  #ifndef __UADDSUBX
    #define __UADDSUBX    __iar_builtin_UADDSUBX
  #endif
  #ifndef __USUBADDX
    #define __USUBADDX    __iar_builtin_USUBADDX
  #endif
  #ifndef __UHADD8
    #define __UHADD8      __iar_builtin_UHADD8
  #endif
  #ifndef __UHADD16
    #define __UHADD16     __iar_builtin_UHADD16
  #endif
  #ifndef __UHSUB8
    #define __UHSUB8      __iar_builtin_UHSUB8
  #endif
  #ifndef __UHSUB16
    #define __UHSUB16     __iar_builtin_UHSUB16
  #endif
  #ifndef __UHADDSUBX
    #define __UHADDSUBX   __iar_builtin_UHADDSUBX
  #endif
  #ifndef __UHSUBADDX
    #define __UHSUBADDX   __iar_builtin_UHSUBADDX
  #endif
  #ifndef __UQADD8
    #define __UQADD8      __iar_builtin_UQADD8
  #endif
  #ifndef __UQADD16
    #define __UQADD16     __iar_builtin_UQADD16
  #endif
  #ifndef __UQSUB8
    #define __UQSUB8      __iar_builtin_UQSUB8
  #endif
  #ifndef __UQSUB16
    #define __UQSUB16     __iar_builtin_UQSUB16
  #endif
  #ifndef __UQADDSUBX
    #define __UQADDSUBX   __iar_builtin_UQADDSUBX
  #endif
  #ifndef __UQSUBADDX
    #define __UQSUBADDX   __iar_builtin_UQSUBADDX
  #endif
  #ifndef __USAD8
    #define __USAD8       __iar_builtin_USAD8
  #endif
  #ifndef __USADA8
    #define __USADA8      __iar_builtin_USADA8
  #endif
  #ifndef __SSAT16
    #define __SSAT16      __iar_builtin_SSAT16
  #endif
  #ifndef __USAT16
    #define __USAT16      __iar_builtin_USAT16
  #endif
  #ifndef __SMUAD
    #define __SMUAD       __iar_builtin_SMUAD
  #endif
  #ifndef __SMUSD
    #define __SMUSD       __iar_builtin_SMUSD
  #endif
  #ifndef __SMUADX
    #define __SMUADX      __iar_builtin_SMUADX
  #endif
  #ifndef __SMUSDX
    #define __SMUSDX      __iar_builtin_SMUSDX
  #endif
  #ifndef __SMLAD
    #define __SMLAD       __iar_builtin_SMLAD
  #endif
  #ifndef __SMLSD
    #define __SMLSD       __iar_builtin_SMLSD
  #endif
  #ifndef __SMLADX
    #define __SMLADX      __iar_builtin_SMLADX
  #endif
  #ifndef __SMLSDX
    #define __SMLSDX      __iar_builtin_SMLSDX
  #endif
  #ifndef __SMLALD
    #define __SMLALD      __iar_builtin_SMLALD
  #endif
  #ifndef __SMLALDX
    #define __SMLALDX     __iar_builtin_SMLALDX
  #endif
  #ifndef __SMLSLD
    #define __SMLSLD      __iar_builtin_SMLSLD
  #endif
  #ifndef __SMLSLDX
    #define __SMLSLDX     __iar_builtin_SMLSLDX
  #endif
  #ifndef __PKHBT
    #define __PKHBT       __iar_builtin_PKHBT
  #endif
  #ifndef __PKHTB
    #define __PKHTB       __iar_builtin_PKHTB
  #endif
  #ifndef __SMLABB
    #define __SMLABB      __iar_builtin_SMLABB
  #endif
  #ifndef __SMLABT
    #define __SMLABT      __iar_builtin_SMLABT
  #endif
  #ifndef __SMLATB
    #define __SMLATB      __iar_builtin_SMLATB
  #endif
  #ifndef __SMLATT
    #define __SMLATT      __iar_builtin_SMLATT
  #endif
  #ifndef __SMLAWB
    #define __SMLAWB      __iar_builtin_SMLAWB
  #endif
  #ifndef __SMLAWT
    #define __SMLAWT      __iar_builtin_SMLAWT
  #endif
  #ifndef __SMMLA
    #define __SMMLA       __iar_builtin_SMMLA
  #endif
  #ifndef __SMMLAR
    #define __SMMLAR      __iar_builtin_SMMLAR
  #endif
  #ifndef __SMMLS
    #define __SMMLS       __iar_builtin_SMMLS
  #endif
  #ifndef __SMMLSR
    #define __SMMLSR      __iar_builtin_SMMLSR
  #endif
  #ifndef __SMMUL
    #define __SMMUL       __iar_builtin_SMMUL
  #endif
  #ifndef __SMMULR
    #define __SMMULR      __iar_builtin_SMMULR
  #endif
  #ifndef __SMULBB
    #define __SMULBB      __iar_builtin_SMULBB
  #endif
  #ifndef __SMULBT
    #define __SMULBT      __iar_builtin_SMULBT
  #endif
  #ifndef __SMULTB
    #define __SMULTB      __iar_builtin_SMULTB
  #endif
  #ifndef __SMULTT
    #define __SMULTT      __iar_builtin_SMULTT
  #endif
  #ifndef __SMULWB
    #define __SMULWB      __iar_builtin_SMULWB
  #endif
  #ifndef __SMULWT
    #define __SMULWT      __iar_builtin_SMULWT
  #endif
  #ifndef __SXTAB
    #define __SXTAB       __iar_builtin_SXTAB
  #endif
  #ifndef __SXTAH
    #define __SXTAH       __iar_builtin_SXTAH
  #endif
  #ifndef __UXTAB
    #define __UXTAB       __iar_builtin_UXTAB
  #endif
  #ifndef __UXTAH
    #define __UXTAH       __iar_builtin_UXTAH
  #endif
  #ifndef __UMAAL
    #define __UMAAL       __iar_builtin_UMAAL
  #endif
  #ifndef __SMLALBB
    #define __SMLALBB     __iar_builtin_SMLALBB
  #endif
  #ifndef __SMLALBT
    #define __SMLALBT     __iar_builtin_SMLALBT
  #endif
  #ifndef __SMLALTB
    #define __SMLALTB     __iar_builtin_SMLALTB
  #endif
  #ifndef __SMLALTT
    #define __SMLALTT     __iar_builtin_SMLALTT
  #endif
  #ifndef __UXTB16
    #define __UXTB16      __iar_builtin_UXTB16
  #endif
  #ifndef __UXTAB16
    #define __UXTAB16     __iar_builtin_UXTAB16
  #endif
  #ifndef __SXTB16
    #define __SXTB16      __iar_builtin_SXTB16
  #endif
  #ifndef __SXTAB16
    #define __SXTAB16     __iar_builtin_SXTAB16
  #endif
  #ifndef __SASX
    #define __SASX        __iar_builtin_SASX
  #endif
  #ifndef __SSAX
    #define __SSAX        __iar_builtin_SSAX
  #endif
  #ifndef __SHASX
    #define __SHASX       __iar_builtin_SHASX
  #endif
  #ifndef __SHSAX
    #define __SHSAX       __iar_builtin_SHSAX
  #endif
  #ifndef __QASX
    #define __QASX        __iar_builtin_QASX
  #endif
  #ifndef __QSAX
    #define __QSAX        __iar_builtin_QSAX
  #endif
  #ifndef __UASX
    #define __UASX        __iar_builtin_UASX
  #endif
  #ifndef __USAX
    #define __USAX        __iar_builtin_USAX
  #endif
  #ifndef __UHASX
    #define __UHASX       __iar_builtin_UHASX
  #endif
  #ifndef __UHSAX
    #define __UHSAX       __iar_builtin_UHSAX
  #endif
  #ifndef __UQASX
    #define __UQASX       __iar_builtin_UQASX
  #endif
  #ifndef __UQSAX
    #define __UQSAX       __iar_builtin_UQSAX
  #endif
#endif /* __ARM_MEDIA__ */

/* Architecture v7 instructions */
#ifndef __DMB
  #define __DMB() __iar_builtin_DMBx(15)
#endif
#ifndef __DSB
  #define __DSB() __iar_builtin_DSBx(15)
#endif
#ifndef __ISB
  #define __ISB() __iar_builtin_ISBx(15)
#endif

/* Architecture v8-M instructions */
#ifndef __TT
  #define __TT    __iar_builtin_TT
#endif
#ifndef __TTT
  #define __TTT   __iar_builtin_TTT
#endif
#ifndef __TTA
  #define __TTA   __iar_builtin_TTA
#endif
#ifndef __TTAT
  #define __TTAT  __iar_builtin_TTAT
#endif

#ifndef __fabs
  #define __fabs(x)  fabs(x)
#endif
#ifndef __fabsf
  #define __fabsf(x) fabsf(x)
#endif

#ifndef __get_SB
  #define __get_SB       __iar_builtin_get_SB
#endif

#ifndef __set_SB
  #define __set_SB       __iar_builtin_set_SB
#endif

#endif  /* __INTRINSICS_INCLUDED */
