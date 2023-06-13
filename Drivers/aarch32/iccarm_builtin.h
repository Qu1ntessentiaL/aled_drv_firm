/**************************************************
 *
 * This file declares the ICCARM builtin functions.
 *
 * Copyright 1999-2017 IAR Systems. All rights reserved.
 *
 * $Revision$
 *
 **************************************************/

#ifndef __BUILTINS_INCLUDED
#define __BUILTINS_INCLUDED

#include <yvals.h>

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/*
 * Check that the correct C compiler is used.
 */

#ifndef __ICCARM__
#error "File iccarm_builtin.h can only be used together with iccarm."
#endif

/* Define function effects for intrinsics */
#define __IEFF_HS _Pragma("function_effects = hidden_state, always_returns")
#define __IEFF_NE _Pragma("function_effects = no_state, always_returns")

#ifdef __cplusplus
extern "C" {
#endif

#pragma language=save
#pragma language=extended

__ATTRIBUTES void    __iar_builtin_no_operation(void);

__ATTRIBUTES void    __iar_builtin_disable_interrupt(void);
__ATTRIBUTES void    __iar_builtin_enable_interrupt(void);

typedef unsigned int __istate_t;

__ATTRIBUTES __istate_t __iar_builtin_get_interrupt_state(void);
__ATTRIBUTES void __iar_builtin_set_interrupt_state(__istate_t);

/* System control access for Cortex-M cores */
__ATTRIBUTES unsigned int __iar_builtin_get_PSR( void );
__ATTRIBUTES unsigned int __iar_builtin_get_IPSR( void );
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_get_MSP( void );
__ATTRIBUTES void         __iar_builtin_set_MSP( unsigned int );
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_get_PSP( void );
__ATTRIBUTES void         __iar_builtin_set_PSP( unsigned int );
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_get_PRIMASK( void );
__ATTRIBUTES void         __iar_builtin_set_PRIMASK( unsigned int );
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_get_CONTROL( void );
__ATTRIBUTES void         __iar_builtin_set_CONTROL( unsigned int );

/* These are only available for v7M */
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_get_FAULTMASK( void );
__ATTRIBUTES void         __iar_builtin_set_FAULTMASK(unsigned int);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_get_BASEPRI( void );
__ATTRIBUTES void         __iar_builtin_set_BASEPRI( unsigned int );

/* "Old" style intrerrupt control routines */
__ATTRIBUTES void __iar_builtin_disable_irq(void);
__ATTRIBUTES void __iar_builtin_enable_irq(void);

__ATTRIBUTES void __iar_builtin_disable_fiq(void);
__ATTRIBUTES void __iar_builtin_enable_fiq(void);


/* ARM-mode intrinsics */

__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SWP( unsigned int, volatile unsigned int * );
__IEFF_NE __ATTRIBUTES unsigned char __iar_builtin_SWPB( unsigned char, volatile unsigned char * );

typedef unsigned int __ul;
typedef unsigned int __iar_builtin_uint;

#define __cpid   unsigned __constrange(0,15)
#define __cpreg  unsigned __constrange(0,15)
#define __cpopcw unsigned __constrange(0,15)
#define __cpopc  unsigned __constrange(0,7)

/*  Co-processor operations */

__IEFF_HS __ATTRIBUTES void __iar_builtin_CDP (__cpid coproc, __cpopcw opc1, __cpreg CRd, __cpreg CRn, __cpreg CRm, __cpopc opc2);
__IEFF_HS __ATTRIBUTES void __iar_builtin_CDP2(__cpid coproc, __cpopcw opc1, __cpreg CRd, __cpreg CRn, __cpreg CRm, __cpopc opc2);

/*  Co-processor access */
__ATTRIBUTES void          __iar_builtin_MCR( __cpid coproc, __cpopc opcode_1, __iar_builtin_uint src,
                                __cpreg CRn, __cpreg CRm, __cpopc opcode_2 );
__ATTRIBUTES unsigned int __iar_builtin_MRC( __cpid coproc, __cpopc opcode_1,
                                __cpreg CRn, __cpreg CRm, __cpopc opcode_2 );
__ATTRIBUTES void          __iar_builtin_MCR2( __cpid coproc, __cpopc opcode_1, __iar_builtin_uint src,
                                 __cpreg CRn, __cpreg CRm, __cpopc opcode_2 );
__ATTRIBUTES unsigned int __iar_builtin_MRC2( __cpid coproc, __cpopc opcode_1,
                                 __cpreg CRn, __cpreg CRm, __cpopc opcode_2 );

__ATTRIBUTES void __iar_builtin_MCRR (__cpid coproc, __cpopcw opc1, unsigned long long src, __cpreg CRm);
__ATTRIBUTES void __iar_builtin_MCRR2(__cpid coproc, __cpopcw opc1, unsigned long long src, __cpreg CRm);

__ATTRIBUTES unsigned long long __iar_builtin_MRRC (__cpid coproc, __cpopcw opc1, __cpreg CRm);
__ATTRIBUTES unsigned long long __iar_builtin_MRRC2(__cpid coproc, __cpopcw opc1, __cpreg CRm);

/* Load coprocessor register. */
__ATTRIBUTES void __iar_builtin_LDC  ( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint const *src);
__ATTRIBUTES void __iar_builtin_LDCL ( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint const *src);
__ATTRIBUTES void __iar_builtin_LDC2 ( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint const *src);
__ATTRIBUTES void __iar_builtin_LDC2L( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint const *src);

/* Store coprocessor register. */
__ATTRIBUTES void __iar_builtin_STC  ( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint *dst);
__ATTRIBUTES void __iar_builtin_STCL ( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint *dst);
__ATTRIBUTES void __iar_builtin_STC2 ( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint *dst);
__ATTRIBUTES void __iar_builtin_STC2L( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint *dst);

/* Load coprocessor register (noindexed version with coprocessor option). */
__ATTRIBUTES void __iar_builtin_LDC_noidx( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint const *src,
                              unsigned __constrange(0,255) option);

__ATTRIBUTES void __iar_builtin_LDCL_noidx( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint const *src,
                               unsigned __constrange(0,255) option);

__ATTRIBUTES void __iar_builtin_LDC2_noidx( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint const *src,
                               unsigned __constrange(0,255) option);

__ATTRIBUTES void __iar_builtin_LDC2L_noidx( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint const *src,
                                unsigned __constrange(0,255) option);

/* Store coprocessor register (version with coprocessor option). */
__ATTRIBUTES void __iar_builtin_STC_noidx( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint *dst,
                              unsigned __constrange(0,255) option);

__ATTRIBUTES void __iar_builtin_STCL_noidx( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint *dst,
                               unsigned __constrange(0,255) option);

__ATTRIBUTES void __iar_builtin_STC2_noidx( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint *dst,
                               unsigned __constrange(0,255) option);

__ATTRIBUTES void __iar_builtin_STC2L_noidx( __cpid coproc, __cpreg CRn, volatile __iar_builtin_uint *dst,
                                unsigned __constrange(0,255) option);

#define __sys_reg  __spec_string const char *

/* Reads a system register */
__ATTRIBUTES unsigned int       __iar_builtin_rsr(__sys_reg special_register);
__ATTRIBUTES unsigned long long __iar_builtin_rsr64(__sys_reg special_register);
__ATTRIBUTES void*              __iar_builtin_rsrp(__sys_reg special_register);

/* Writes a system register */
__ATTRIBUTES void __iar_builtin_wsr(__sys_reg special_register, unsigned int value);
__ATTRIBUTES void __iar_builtin_wsr64(__sys_reg special_register, unsigned long long value);
__ATTRIBUTES void __iar_builtin_wsrp(__sys_reg special_register, const void *value);

/* Status register access, v7M: */
__ATTRIBUTES unsigned int __iar_builtin_get_APSR( void );
__ATTRIBUTES void         __iar_builtin_set_APSR( unsigned int );

/* Status register access */
__ATTRIBUTES unsigned int __iar_builtin_get_CPSR( void );
__ATTRIBUTES void         __iar_builtin_set_CPSR( unsigned int );

/* Floating-point status and control register access */
__ATTRIBUTES unsigned int __iar_builtin_get_FPSCR( void );
__ATTRIBUTES void __iar_builtin_set_FPSCR( unsigned int );

/* Architecture v5T, CLZ is also available in Thumb mode for Thumb2 cores */
/* For other architecture an instruction sequence is emitted */
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_CLZ(unsigned int);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_ROR(unsigned int, unsigned int);
__ATTRIBUTES unsigned int __iar_builtin_RRX(unsigned int);

/* Architecture v5TE or profile M with DSP-extension */
__IEFF_HS __ATTRIBUTES signed int __iar_builtin_QADD( signed int, signed int );
__IEFF_HS __ATTRIBUTES signed int __iar_builtin_QDADD( signed int, signed int );
__IEFF_HS __ATTRIBUTES signed int __iar_builtin_QSUB( signed int, signed int );
__IEFF_HS __ATTRIBUTES signed int __iar_builtin_QDSUB( signed int, signed int );
__IEFF_HS __ATTRIBUTES signed int __iar_builtin_QDOUBLE( signed int );
__IEFF_HS __ATTRIBUTES int        __iar_builtin_QFlag( void );
__IEFF_HS __ATTRIBUTES int   __iar_builtin_acle_QFlag(void);
__IEFF_HS __ATTRIBUTES void  __iar_builtin_set_QFlag(int);
__IEFF_HS __ATTRIBUTES void  __iar_builtin_ignore_QFlag(void);

__IEFF_HS __ATTRIBUTES int         __iar_builtin_QCFlag( void );
__IEFF_HS __ATTRIBUTES void __iar_builtin_reset_QC_flag( void );

__IEFF_NE __ATTRIBUTES signed int __iar_builtin_SMUL( signed short, signed short );

/* Architecture v6, REV and REVSH are also available in thumb mode */
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_REV( unsigned int );
__IEFF_NE __ATTRIBUTES signed int __iar_builtin_REVSH( short );

__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_REV16( unsigned int );
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_RBIT( unsigned int );

__ATTRIBUTES unsigned char  __iar_builtin_LDREXB( volatile unsigned char const * );
__ATTRIBUTES unsigned short __iar_builtin_LDREXH( volatile unsigned short const * );
__ATTRIBUTES unsigned int  __iar_builtin_LDREX ( volatile unsigned int const * );
__ATTRIBUTES unsigned long long __iar_builtin_LDREXD( volatile unsigned long long const * );

__ATTRIBUTES unsigned int  __iar_builtin_STREXB( unsigned char, volatile unsigned char * );
__ATTRIBUTES unsigned int  __iar_builtin_STREXH( unsigned short, volatile unsigned short * );
__ATTRIBUTES unsigned int  __iar_builtin_STREX ( unsigned int, volatile unsigned int * );
__ATTRIBUTES unsigned int  __iar_builtin_STREXD( unsigned long long, volatile unsigned long long * );

__ATTRIBUTES void __iar_builtin_CLREX( void );

__ATTRIBUTES void __iar_builtin_SEV( void );
__ATTRIBUTES void __iar_builtin_WFE( void );
__ATTRIBUTES void __iar_builtin_WFI( void );
__ATTRIBUTES void __iar_builtin_YIELD( void );

__ATTRIBUTES void __iar_builtin_PLI( volatile void const * );
__ATTRIBUTES void __iar_builtin_PLD( volatile void const * );

__ATTRIBUTES void __iar_builtin_PLIx( volatile void const *, unsigned int __constrange(0,2), unsigned int __constrange(0,1));
__ATTRIBUTES void __iar_builtin_PLDx( volatile void const *, unsigned int __constrange(0, 1), unsigned int __constrange(0, 2), unsigned int __constrange(0, 1));
__ATTRIBUTES void __iar_builtin_PLDW( volatile void const * );

__IEFF_HS __ATTRIBUTES signed int   __iar_builtin_SSAT     (signed int val, unsigned int __constrange( 1, 32 ) sat );
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_USAT     (signed int val, unsigned int __constrange( 0, 31 ) sat );

/* Architecture v6 Media instructions */
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SEL( unsigned int op1, unsigned int op2 );

__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SADD8    (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SADD16   (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SSUB8    (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SSUB16   (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SADDSUBX (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SSUBADDX (unsigned int pair1, unsigned int pair2);

__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SHADD8   (unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SHADD16  (unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SHSUB8   (unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SHSUB16  (unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SHADDSUBX(unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SHSUBADDX(unsigned int pair1, unsigned int pair2);

__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_QADD8    (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_QADD16   (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_QSUB8    (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_QSUB16   (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_QADDSUBX (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_QSUBADDX (unsigned int pair1, unsigned int pair2);

__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UADD8    (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UADD16   (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_USUB8    (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_USUB16   (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UADDSUBX (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_USUBADDX (unsigned int pair1, unsigned int pair2);

__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UHADD8   (unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UHADD16  (unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UHSUB8   (unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UHSUB16  (unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UHADDSUBX(unsigned int pair1, unsigned int pair2);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UHSUBADDX(unsigned int pair1, unsigned int pair2);

__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UQADD8   (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UQADD16  (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UQSUB8   (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UQSUB16  (unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UQADDSUBX(unsigned int pair1, unsigned int pair2);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UQSUBADDX(unsigned int pair1, unsigned int pair2);

__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_USAD8(unsigned int x, unsigned int y );
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_USADA8(unsigned int x, unsigned int y,
                                   unsigned int acc );

__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SSAT16   (unsigned int pair,
                                      unsigned int __constrange( 1, 16 ) sat );
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_USAT16   (unsigned int pair,
                                      unsigned int __constrange( 0, 15 ) sat );

__IEFF_HS __ATTRIBUTES int __iar_builtin_SMUAD (unsigned int x, unsigned int y);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMUSD (unsigned int x, unsigned int y);
__IEFF_HS __ATTRIBUTES int __iar_builtin_SMUADX(unsigned int x, unsigned int y);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMUSDX(unsigned int x, unsigned int y);

__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLAD (unsigned int x, unsigned int y, int sum);
__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLSD (unsigned int x, unsigned int y, int sum);
__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLADX(unsigned int x, unsigned int y, int sum);
__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLSDX(unsigned int x, unsigned int y, int sum);

__IEFF_NE __ATTRIBUTES long long __iar_builtin_SMLALD (unsigned int pair1,
                                 unsigned int pair2,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __iar_builtin_SMLALDX(unsigned int pair1,
                                 unsigned int pair2,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __iar_builtin_SMLSLD (unsigned int pair1,
                                 unsigned int pair2,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __iar_builtin_SMLSLDX(unsigned int pair1,
                                 unsigned int pair2,
                                 long long acc);

__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_PKHBT(unsigned int x,
                                  unsigned int y,
                                  unsigned __constrange(0,31) count);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_PKHTB(unsigned int x,
                                  unsigned int y,
                                  unsigned __constrange(0,32) count);

__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLABB(unsigned int x, unsigned int y, int acc);
__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLABT(unsigned int x, unsigned int y, int acc);
__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLATB(unsigned int x, unsigned int y, int acc);
__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLATT(unsigned int x, unsigned int y, int acc);

__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLAWB(int x, unsigned int y, int acc);
__IEFF_HS __ATTRIBUTES int __iar_builtin_SMLAWT(int x, unsigned int y, int acc);

__IEFF_NE __ATTRIBUTES int __iar_builtin_SMMLA (int x, int y, int acc);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMMLAR(int x, int y, int acc);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMMLS (int x, int y, int acc);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMMLSR(int x, int y, int acc);

__IEFF_NE __ATTRIBUTES int __iar_builtin_SMMUL (int x, int y);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMMULR(int x, int y);

__IEFF_NE __ATTRIBUTES int __iar_builtin_SMULBB(unsigned int x, unsigned int y);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMULBT(unsigned int x, unsigned int y);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMULTB(unsigned int x, unsigned int y);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMULTT(unsigned int x, unsigned int y);

__IEFF_NE __ATTRIBUTES int __iar_builtin_SMULWB(int x, unsigned int y);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SMULWT(int x, unsigned int y);

__IEFF_NE __ATTRIBUTES int __iar_builtin_SXTAB (int x, unsigned int y);
__IEFF_NE __ATTRIBUTES int __iar_builtin_SXTAH (int x, unsigned int y);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UXTAB (unsigned int x, unsigned int y);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UXTAH (unsigned int x, unsigned int y);

__IEFF_NE __ATTRIBUTES unsigned long long __iar_builtin_UMAAL(unsigned int x,
                                       unsigned int y,
                                       unsigned int a,
                                       unsigned int b);

__IEFF_NE __ATTRIBUTES long long __iar_builtin_SMLALBB(unsigned int x,
                                 unsigned int y,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __iar_builtin_SMLALBT(unsigned int x,
                                 unsigned int y,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __iar_builtin_SMLALTB(unsigned int x,
                                 unsigned int y,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __iar_builtin_SMLALTT(unsigned int x,
                                 unsigned int y,
                                 long long acc);

__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UXTB16(unsigned int x);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UXTAB16(unsigned int acc, unsigned int x);

__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SXTB16(unsigned int x);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SXTAB16(unsigned int acc, unsigned int x);

__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SASX(unsigned int, unsigned int);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_SSAX(unsigned int, unsigned int);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SHASX(unsigned int, unsigned int);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_SHSAX(unsigned int, unsigned int);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_QASX(unsigned int, unsigned int);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_QSAX(unsigned int, unsigned int);

__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UASX(unsigned int, unsigned int);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_USAX(unsigned int, unsigned int);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UHASX(unsigned int, unsigned int);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_UHSAX(unsigned int, unsigned int);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UQASX(unsigned int, unsigned int);
__IEFF_HS __ATTRIBUTES unsigned int __iar_builtin_UQSAX(unsigned int, unsigned int);

/* Architecture v7 instructions */
__ATTRIBUTES void __iar_builtin_DMB(void);
__ATTRIBUTES void __iar_builtin_DSB(void);
__ATTRIBUTES void __iar_builtin_ISB(void);
__ATTRIBUTES void __iar_builtin_DMBx(unsigned int __constrange(1, 15));
__ATTRIBUTES void __iar_builtin_DSBx(unsigned int __constrange(1, 15));
__ATTRIBUTES void __iar_builtin_ISBx(unsigned int __constrange(1, 15));

/* Architecture v8-M instructions */
__ATTRIBUTES unsigned int __iar_builtin_TT(unsigned int);
__ATTRIBUTES unsigned int __iar_builtin_TTT(unsigned int);
__ATTRIBUTES unsigned int __iar_builtin_TTA(unsigned int);
__ATTRIBUTES unsigned int __iar_builtin_TTAT(unsigned int);

__ATTRIBUTES unsigned int __iar_builtin_get_SB(void);
__ATTRIBUTES void __iar_builtin_set_SB(unsigned int);

__ATTRIBUTES unsigned int __get_LR(void);
__ATTRIBUTES void __set_LR(unsigned int);

__ATTRIBUTES unsigned int __get_SP(void);
__ATTRIBUTES void __set_SP(unsigned int);

/* VFP: sqrt */
__IEFF_NE __ATTRIBUTES float __iar_builtin_VSQRT_F32(float x);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VSQRT_F64(double x);

/* VFPv4: fused mac */
__IEFF_NE __ATTRIBUTES float __iar_builtin_VFMA_F32(float x, float y, float z);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VFMS_F32(float x, float y, float z);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VFNMA_F32(float x, float y, float z);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VFNMS_F32(float x, float y, float z);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VFMA_F64(double x, double y, double z);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VFMS_F64(double x, double y, double z);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VFNMA_F64(double x, double y, double z);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VFNMS_F64(double x, double y, double z);

/* Architecture v8-A/R: CRC extension */
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_CRC32B(unsigned int crc, unsigned char data);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_CRC32H(unsigned int crc, unsigned short data);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_CRC32W(unsigned int crc, unsigned int data);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_CRC32CB(unsigned int crc, unsigned char data);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_CRC32CH(unsigned int crc, unsigned short data);
__IEFF_NE __ATTRIBUTES unsigned int __iar_builtin_CRC32CW(unsigned int crc, unsigned int data);

/* VFPv5: numerical min/max */
__IEFF_NE __ATTRIBUTES float __iar_builtin_VMAXNM_F32(float a, float b);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VMINNM_F32(float a, float b);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VMAXNM_F64(double a, double b);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VMINNM_F64(double a, double b);

/* VFPv5: rounding */
__IEFF_NE __ATTRIBUTES float __iar_builtin_VRINTA_F32(float a);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VRINTM_F32(float a);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VRINTN_F32(float a);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VRINTP_F32(float a);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VRINTX_F32(float a);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VRINTR_F32(float a);
__IEFF_NE __ATTRIBUTES float __iar_builtin_VRINTZ_F32(float a);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VRINTA_F64(double a);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VRINTM_F64(double a);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VRINTN_F64(double a);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VRINTP_F64(double a);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VRINTX_F64(double a);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VRINTR_F64(double a);
__IEFF_NE __ATTRIBUTES double __iar_builtin_VRINTZ_F64(double a);

#pragma language=restore

#ifdef __cplusplus
}
#endif

#undef __cpid
#undef __cpreg
#undef __cpopcw
#undef __cpopc
#undef __sys_reg

#undef __IEFF_HS
#undef __IEFF_NE

#endif  /* __BUILTINS_INCLUDED */
