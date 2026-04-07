/****************************************************************************
*  Copyright 2025 ZJY (Email:3102196558@qq.com)                             *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

#ifndef __GUI_LV_USER_ARCH_PORT_H__
#define __GUI_LV_USER_ARCH_PORT_H__

/*================================= INCLUDES =================================*/
#include <stdint.h>

#ifdef   __cplusplus
extern "C" {
#endif
/*================================== MACROS ==================================*/

/*----------------------------------------------------------------------------*
 * Environment Detection                                                      *
 *----------------------------------------------------------------------------*/
/* detect IAR */
#undef __IS_COMPILER_IAR__
#if defined(__IAR_SYSTEMS_ICC__)
#   define __IS_COMPILER_IAR__                  1
#endif

/* detect arm compiler 5 */
#undef __IS_COMPILER_ARM_COMPILER_5__
#if ((__ARMCC_VERSION >= 5000000) && (__ARMCC_VERSION < 6000000))
#   define __IS_COMPILER_ARM_COMPILER_5__       1
#endif

/* detect arm compiler 6 */
#undef __IS_COMPILER_ARM_COMPILER_6__
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#   define __IS_COMPILER_ARM_COMPILER_6__       1
#endif

/* detect arm compilers */
#undef __IS_COMPILER_ARM_COMPILER__
#if defined(__IS_COMPILER_ARM_COMPILER_5__) && __IS_COMPILER_ARM_COMPILER_5__  \
||  defined(__IS_COMPILER_ARM_COMPILER_6__) && __IS_COMPILER_ARM_COMPILER_6__
#   define __IS_COMPILER_ARM_COMPILER__         1
#endif

/* detect clang (llvm) */
#undef  __IS_COMPILER_LLVM__
#if defined(__clang__) && !defined(__IS_COMPILER_ARM_COMPILER_6__)
#   define __IS_COMPILER_LLVM__                 1
#else

/* detect gcc */
#   undef __IS_COMPILER_GCC__
#   if defined(__GNUC__) && !(  defined(__IS_COMPILER_ARM_COMPILER__)          \
							||  defined(__IS_COMPILER_LLVM__)                  \
							||  defined(__IS_COMPILER_IAR__))
#       define __IS_COMPILER_GCC__              1
#   endif

#endif

/*----------------------------------------------------------------------------*
 * Architecture Port                                                          *
 *----------------------------------------------------------------------------*/
#if defined(_MSC_VER)
#   define __STATIC_FORCEINLINE               static __forceinline
#   define __STATIC_INLINE                    static __inline
#   define __ALIGNED(__N)                     __declspec(align(__N))
#   define __WEAK
#elif defined(__APPLE_CC__)
#   define __STATIC_FORCEINLINE               static inline                    \
											 __attribute__((always_inline))
#   define __STATIC_INLINE                    static inline
#   define __ALIGNED(__N)                     __attribute__((aligned(__N)))
#   define __WEAK                             __attribute__((weak))
#else
#   define __STATIC_FORCEINLINE               static inline                    \
											 __attribute__((always_inline))
#   define __STATIC_INLINE                    static inline
#   define __ALIGNED(__N)                     __attribute__((aligned(__N)))
#   define __WEAK                             __attribute__((weak))
#endif

#ifndef GUI_LV_ALIGNOF
#   if defined(__cplusplus) && (__cplusplus >= 201103L)
#       define GUI_LV_ALIGNOF(__TYPE)         alignof(__TYPE)
#   elif defined(_MSC_VER)
#       define GUI_LV_ALIGNOF(__TYPE)         __alignof(__TYPE)
#   elif defined(__IAR_SYSTEMS_ICC__)
#       define GUI_LV_ALIGNOF(__TYPE)         __ALIGNOF__(__TYPE)
#   elif defined(__clang__) || defined(__GNUC__)                               \
		|| defined(__ARMCC_VERSION)
#       define GUI_LV_ALIGNOF(__TYPE)         __alignof__(__TYPE)
#   elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#       define GUI_LV_ALIGNOF(__TYPE)         _Alignof(__TYPE)
#   else
#       define GUI_LV_ALIGNOF(__TYPE)                                          \
			(sizeof(struct {                                                   \
				char   chDummy;                                                \
				__TYPE tObject;                                                \
			}) - sizeof(__TYPE))
#   endif
#endif

/*----------------------------------------------------------------------------*
 * Compiler Attributes                                                        *
 *----------------------------------------------------------------------------*/
#ifndef FALL_THROUGH
#   if defined(__cplusplus) && (__cplusplus >= 201703L)
#       define FALL_THROUGH               [[fallthrough]]
#   elif defined(__GNUC__) && (__GNUC__ >= 7)
#       define FALL_THROUGH               __attribute__((fallthrough))
#   elif defined(__clang__) && defined(__has_attribute)
#       if __has_attribute(fallthrough)
#           define FALL_THROUGH           __attribute__((fallthrough))
#       else
#           define FALL_THROUGH           ((void)0)
#       endif
#   else
#       define FALL_THROUGH               ((void)0)
#   endif
#endif

#ifndef GUI_LV_WEAK
#   define GUI_LV_WEAK                        __WEAK
#endif

#ifndef GUI_LV_NONNULL
#   if defined(__IS_COMPILER_ARM_COMPILER_5__)                               \
	|| defined(__IS_COMPILER_ARM_COMPILER_6__)                               \
	|| defined(__IS_COMPILER_GCC__)                                          \
	|| defined(__IS_COMPILER_LLVM__)
#       define GUI_LV_NONNULL(...)           __attribute__((nonnull(__VA_ARGS__)))
#   else
#       define GUI_LV_NONNULL(...)
#   endif
#endif

/*----------------------------------------------------------------------------*
 * Macro Utilities                                                            *
 *----------------------------------------------------------------------------*/
#define __GUI_LV_CONNECT2(__A, __B)            __A##__B
#define __GUI_LV_CONNECT3(__A, __B, __C)       __A##__B##__C
#define __GUI_LV_CONNECT4(__A, __B, __C, __D)  __A##__B##__C##__D

#define GUI_LV_CONNECT2(__A, __B)              __GUI_LV_CONNECT2(__A, __B)
#define GUI_LV_CONNECT3(__A, __B, __C)         __GUI_LV_CONNECT3(__A, __B, __C)
#define GUI_LV_CONNECT4(__A, __B, __C, __D)    __GUI_LV_CONNECT4(__A,        \
																  __B,        \
																  __C,        \
																  __D)

#ifndef GUI_LV_SAFE_NAME
#   define GUI_LV_SAFE_NAME(__NAME)            GUI_LV_CONNECT3(__,            \
																__LINE__,     \
																__NAME)
#endif
/*============================ MACROFIED FUNCTIONS ===========================*/
/*================================== TYPES ===================================*/
/*============================= GLOBAL VARIABLES =============================*/
/*============================== LOCAL VARIABLES =============================*/
/*================================ PROTOTYPES ================================*/
/*============================== IMPLEMENTATION ==============================*/
/*=================================== END ====================================*/
#ifdef   __cplusplus
}
#endif
#endif /* __GUI_LV_USER_ARCH_PORT_H__ */
