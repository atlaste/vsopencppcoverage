/*===-- X86DisassemblerDecoderCommon.h - Disassembler decoder -----*- C -*-===*
*
*                     The LLVM Compiler Infrastructure
*
* This file is distributed under the University of Illinois Open Source
* License. See LICENSE.TXT for details.
*
*===----------------------------------------------------------------------===*
*
* This file is part of the X86 Disassembler.
* It contains common definitions used by both the disassembler and the table
*  generator.
* Documentation for the disassembler can be found in X86Disassembler.h.
*
*===----------------------------------------------------------------------===*/

/* Capstone Disassembly Engine */
/* By Nguyen Anh Quynh <aquynh@gmail.com>, 2013-2014 */

/*
* This header file provides those definitions that need to be shared between
* the decoder and the table generator in a C-friendly manner.
*/

#ifndef CS_X86_DISASSEMBLERDECODERCOMMON_H
#define CS_X86_DISASSEMBLERDECODERCOMMON_H

#if !defined(_MSC_VER) || !defined(_KERNEL_MODE)
#include <stdint.h>
#endif

#define INSTRUCTIONS_SYM  x86DisassemblerInstrSpecifiers
#define CONTEXTS_SYM      x86DisassemblerContexts
#define ONEBYTE_SYM       x86DisassemblerOneByteOpcodes
#define TWOBYTE_SYM       x86DisassemblerTwoByteOpcodes
#define THREEBYTE38_SYM   x86DisassemblerThreeByte38Opcodes
#define THREEBYTE3A_SYM   x86DisassemblerThreeByte3AOpcodes
#define XOP8_MAP_SYM      x86DisassemblerXOP8Opcodes
#define XOP9_MAP_SYM      x86DisassemblerXOP9Opcodes
#define XOPA_MAP_SYM      x86DisassemblerXOPAOpcodes
#define T3DNOW_MAP_SYM    x86DisassemblerT3DNOWOpcodes


/*
* Attributes of an instruction that must be known before the opcode can be
* processed correctly.  Most of these indicate the presence of particular
* prefixes, but ATTR_64BIT is simply an attribute of the decoding context.
*/
#define ATTRIBUTE_BITS                  \
  ENUM_ENTRY(ATTR_NONE,   0x00)         \
  ENUM_ENTRY(ATTR_64BIT,  (0x1 << 0))   \
  ENUM_ENTRY(ATTR_XS,     (0x1 << 1))   \
  ENUM_ENTRY(ATTR_XD,     (0x1 << 2))   \
  ENUM_ENTRY(ATTR_REXW,   (0x1 << 3))   \
  ENUM_ENTRY(ATTR_OPSIZE, (0x1 << 4))   \
  ENUM_ENTRY(ATTR_ADSIZE, (0x1 << 5))   \
  ENUM_ENTRY(ATTR_VEX,    (0x1 << 6))   \
  ENUM_ENTRY(ATTR_VEXL,   (0x1 << 7))   \
  ENUM_ENTRY(ATTR_EVEX,   (0x1 << 8))   \
  ENUM_ENTRY(ATTR_EVEXL,  (0x1 << 9))   \
  ENUM_ENTRY(ATTR_EVEXL2, (0x1 << 10))  \
  ENUM_ENTRY(ATTR_EVEXK,  (0x1 << 11))  \
  ENUM_ENTRY(ATTR_EVEXKZ, (0x1 << 12))  \
  ENUM_ENTRY(ATTR_EVEXB,  (0x1 << 13))

#define ENUM_ENTRY(n, v) n = v,
enum attributeBits
{
	ATTRIBUTE_BITS
	ATTR_max
};
#undef ENUM_ENTRY

/*
* Combinations of the above attributes that are relevant to instruction
* decode. Although other combinations are possible, they can be reduced to
* these without affecting the ultimately decoded instruction.
*/

/*           Class name           Rank  Rationale for rank assignment         */
#define INSTRUCTION_CONTEXTS                                                   \
	ENUM_ENTRY(IC,                    0,  "says nothing about the instruction")  \
ENUM_ENTRY(IC_64BIT,              1,  "says the instruction applies in "     \
		"64-bit mode but no more")             \
ENUM_ENTRY(IC_OPSIZE,             3,  "requires an OPSIZE prefix, so "       \
		"operands change width")               \
ENUM_ENTRY(IC_ADSIZE,             3,  "requires an ADSIZE prefix, so "       \
		"operands change width")               \
ENUM_ENTRY(IC_OF,                 2,  "requires 0x0F prefix")                 \
ENUM_ENTRY(IC_XD,                 2,  "may say something about the opcode "  \
		"but not the operands")                \
ENUM_ENTRY(IC_XS,                 2,  "may say something about the opcode "  \
		"but not the operands")                \
ENUM_ENTRY(IC_XD_OPSIZE,          3,  "requires an OPSIZE prefix, so "       \
		"operands change width")               \
ENUM_ENTRY(IC_XS_OPSIZE,          3,  "requires an OPSIZE prefix, so "       \
		"operands change width")               \
ENUM_ENTRY(IC_64BIT_REXW,         4,  "requires a REX.W prefix, so operands "\
		"change width; overrides IC_OPSIZE")   \
ENUM_ENTRY(IC_64BIT_OPSIZE,       3,  "Just as meaningful as IC_OPSIZE")     \
ENUM_ENTRY(IC_64BIT_ADSIZE,       3,  "Just as meaningful as IC_ADSIZE")     \
ENUM_ENTRY(IC_64BIT_XD,           5,  "XD instructions are SSE; REX.W is "   \
		"secondary")                           \
ENUM_ENTRY(IC_64BIT_XS,           5,  "Just as meaningful as IC_64BIT_XD")   \
ENUM_ENTRY(IC_64BIT_XD_OPSIZE,    3,  "Just as meaningful as IC_XD_OPSIZE")  \
ENUM_ENTRY(IC_64BIT_XS_OPSIZE,    3,  "Just as meaningful as IC_XS_OPSIZE")  \
ENUM_ENTRY(IC_64BIT_REXW_XS,      6,  "OPSIZE could mean a different "       \
		"opcode")                              \
ENUM_ENTRY(IC_64BIT_REXW_XD,      6,  "Just as meaningful as "               \
		"IC_64BIT_REXW_XS")                    \
ENUM_ENTRY(IC_64BIT_REXW_OPSIZE,  7,  "The Dynamic Duo!  Prefer over all "   \
		"else because this changes most "      \
		"operands' meaning")                   \
ENUM_ENTRY(IC_VEX,                1,  "requires a VEX prefix")               \
ENUM_ENTRY(IC_VEX_XS,             2,  "requires VEX and the XS prefix")      \
ENUM_ENTRY(IC_VEX_XD,             2,  "requires VEX and the XD prefix")      \
ENUM_ENTRY(IC_VEX_OPSIZE,         2,  "requires VEX and the OpSize prefix")  \
ENUM_ENTRY(IC_VEX_W,              3,  "requires VEX and the W prefix")       \
ENUM_ENTRY(IC_VEX_W_XS,           4,  "requires VEX, W, and XS prefix")      \
ENUM_ENTRY(IC_VEX_W_XD,           4,  "requires VEX, W, and XD prefix")      \
ENUM_ENTRY(IC_VEX_W_OPSIZE,       4,  "requires VEX, W, and OpSize")         \
ENUM_ENTRY(IC_VEX_L,              3,  "requires VEX and the L prefix")       \
ENUM_ENTRY(IC_VEX_L_XS,           4,  "requires VEX and the L and XS prefix")\
ENUM_ENTRY(IC_VEX_L_XD,           4,  "requires VEX and the L and XD prefix")\
ENUM_ENTRY(IC_VEX_L_OPSIZE,       4,  "requires VEX, L, and OpSize")         \
ENUM_ENTRY(IC_VEX_L_W,            4,  "requires VEX, L and W")               \
ENUM_ENTRY(IC_VEX_L_W_XS,         5,  "requires VEX, L, W and XS prefix")    \
ENUM_ENTRY(IC_VEX_L_W_XD,         5,  "requires VEX, L, W and XD prefix")    \
ENUM_ENTRY(IC_VEX_L_W_OPSIZE,     5,  "requires VEX, L, W and OpSize")       \
ENUM_ENTRY(IC_EVEX,               1,  "requires an EVEX prefix")             \
ENUM_ENTRY(IC_EVEX_XS,            2,  "requires EVEX and the XS prefix")     \
ENUM_ENTRY(IC_EVEX_XD,            2,  "requires EVEX and the XD prefix")     \
ENUM_ENTRY(IC_EVEX_OPSIZE,        2,  "requires EVEX and the OpSize prefix") \
ENUM_ENTRY(IC_EVEX_W,             3,  "requires EVEX and the W prefix")      \
ENUM_ENTRY(IC_EVEX_W_XS,          4,  "requires EVEX, W, and XS prefix")     \
ENUM_ENTRY(IC_EVEX_W_XD,          4,  "requires EVEX, W, and XD prefix")     \
ENUM_ENTRY(IC_EVEX_W_OPSIZE,      4,  "requires EVEX, W, and OpSize")        \
ENUM_ENTRY(IC_EVEX_L,             3,  "requires EVEX and the L prefix")       \
ENUM_ENTRY(IC_EVEX_L_XS,          4,  "requires EVEX and the L and XS prefix")\
ENUM_ENTRY(IC_EVEX_L_XD,          4,  "requires EVEX and the L and XD prefix")\
ENUM_ENTRY(IC_EVEX_L_OPSIZE,      4,  "requires EVEX, L, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L_W,           3,  "requires EVEX, L and W")               \
ENUM_ENTRY(IC_EVEX_L_W_XS,        4,  "requires EVEX, L, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_XD,        4,  "requires EVEX, L, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_OPSIZE,    4,  "requires EVEX, L, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_L2,            3,  "requires EVEX and the L2 prefix")       \
ENUM_ENTRY(IC_EVEX_L2_XS,         4,  "requires EVEX and the L2 and XS prefix")\
ENUM_ENTRY(IC_EVEX_L2_XD,         4,  "requires EVEX and the L2 and XD prefix")\
ENUM_ENTRY(IC_EVEX_L2_OPSIZE,     4,  "requires EVEX, L2, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L2_W,          3,  "requires EVEX, L2 and W")               \
ENUM_ENTRY(IC_EVEX_L2_W_XS,       4,  "requires EVEX, L2, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_XD,       4,  "requires EVEX, L2, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_OPSIZE,   4,  "requires EVEX, L2, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_K,             1,  "requires an EVEX_K prefix")             \
ENUM_ENTRY(IC_EVEX_XS_K,          2,  "requires EVEX_K and the XS prefix")     \
ENUM_ENTRY(IC_EVEX_XD_K,          2,  "requires EVEX_K and the XD prefix")     \
ENUM_ENTRY(IC_EVEX_OPSIZE_K,      2,  "requires EVEX_K and the OpSize prefix") \
ENUM_ENTRY(IC_EVEX_W_K,           3,  "requires EVEX_K and the W prefix")      \
ENUM_ENTRY(IC_EVEX_W_XS_K,        4,  "requires EVEX_K, W, and XS prefix")     \
ENUM_ENTRY(IC_EVEX_W_XD_K,        4,  "requires EVEX_K, W, and XD prefix")     \
ENUM_ENTRY(IC_EVEX_W_OPSIZE_K,    4,  "requires EVEX_K, W, and OpSize")        \
ENUM_ENTRY(IC_EVEX_L_K,           3,  "requires EVEX_K and the L prefix")       \
ENUM_ENTRY(IC_EVEX_L_XS_K,        4,  "requires EVEX_K and the L and XS prefix")\
ENUM_ENTRY(IC_EVEX_L_XD_K,        4,  "requires EVEX_K and the L and XD prefix")\
ENUM_ENTRY(IC_EVEX_L_OPSIZE_K,    4,  "requires EVEX_K, L, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L_W_K,         3,  "requires EVEX_K, L and W")               \
ENUM_ENTRY(IC_EVEX_L_W_XS_K,      4,  "requires EVEX_K, L, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_XD_K,      4,  "requires EVEX_K, L, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_OPSIZE_K,  4,  "requires EVEX_K, L, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_L2_K,          3,  "requires EVEX_K and the L2 prefix")       \
ENUM_ENTRY(IC_EVEX_L2_XS_K,       4,  "requires EVEX_K and the L2 and XS prefix")\
ENUM_ENTRY(IC_EVEX_L2_XD_K,       4,  "requires EVEX_K and the L2 and XD prefix")\
ENUM_ENTRY(IC_EVEX_L2_OPSIZE_K,   4,  "requires EVEX_K, L2, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L2_W_K,        3,  "requires EVEX_K, L2 and W")               \
ENUM_ENTRY(IC_EVEX_L2_W_XS_K,     4,  "requires EVEX_K, L2, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_XD_K,     4,  "requires EVEX_K, L2, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_OPSIZE_K, 4,  "requires EVEX_K, L2, W and OpSize")     \
ENUM_ENTRY(IC_EVEX_B,             1,  "requires an EVEX_B prefix")             \
ENUM_ENTRY(IC_EVEX_XS_B,          2,  "requires EVEX_B and the XS prefix")     \
ENUM_ENTRY(IC_EVEX_XD_B,          2,  "requires EVEX_B and the XD prefix")     \
ENUM_ENTRY(IC_EVEX_OPSIZE_B,      2,  "requires EVEX_B and the OpSize prefix") \
ENUM_ENTRY(IC_EVEX_W_B,           3,  "requires EVEX_B and the W prefix")      \
ENUM_ENTRY(IC_EVEX_W_XS_B,        4,  "requires EVEX_B, W, and XS prefix")     \
ENUM_ENTRY(IC_EVEX_W_XD_B,        4,  "requires EVEX_B, W, and XD prefix")     \
ENUM_ENTRY(IC_EVEX_W_OPSIZE_B,    4,  "requires EVEX_B, W, and OpSize")        \
ENUM_ENTRY(IC_EVEX_L_B,           3,  "requires EVEX_B and the L prefix")       \
ENUM_ENTRY(IC_EVEX_L_XS_B,        4,  "requires EVEX_B and the L and XS prefix")\
ENUM_ENTRY(IC_EVEX_L_XD_B,        4,  "requires EVEX_B and the L and XD prefix")\
ENUM_ENTRY(IC_EVEX_L_OPSIZE_B,    4,  "requires EVEX_B, L, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L_W_B,         3,  "requires EVEX_B, L and W")               \
ENUM_ENTRY(IC_EVEX_L_W_XS_B,      4,  "requires EVEX_B, L, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_XD_B,      4,  "requires EVEX_B, L, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_OPSIZE_B,  4,  "requires EVEX_B, L, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_L2_B,          3,  "requires EVEX_B and the L2 prefix")       \
ENUM_ENTRY(IC_EVEX_L2_XS_B,       4,  "requires EVEX_B and the L2 and XS prefix")\
ENUM_ENTRY(IC_EVEX_L2_XD_B,       4,  "requires EVEX_B and the L2 and XD prefix")\
ENUM_ENTRY(IC_EVEX_L2_OPSIZE_B,   4,  "requires EVEX_B, L2, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L2_W_B,        3,  "requires EVEX_B, L2 and W")               \
ENUM_ENTRY(IC_EVEX_L2_W_XS_B,     4,  "requires EVEX_B, L2, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_XD_B,     4,  "requires EVEX_B, L2, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_OPSIZE_B, 4,  "requires EVEX_B, L2, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_K_B,           1,  "requires EVEX_B and EVEX_K prefix")             \
ENUM_ENTRY(IC_EVEX_XS_K_B,        2,  "requires EVEX_B, EVEX_K and the XS prefix")     \
ENUM_ENTRY(IC_EVEX_XD_K_B,        2,  "requires EVEX_B, EVEX_K and the XD prefix")     \
ENUM_ENTRY(IC_EVEX_OPSIZE_K_B,    2,  "requires EVEX_B, EVEX_K and the OpSize prefix") \
ENUM_ENTRY(IC_EVEX_W_K_B,         3,  "requires EVEX_B, EVEX_K and the W prefix")      \
ENUM_ENTRY(IC_EVEX_W_XS_K_B,      4,  "requires EVEX_B, EVEX_K, W, and XS prefix")     \
ENUM_ENTRY(IC_EVEX_W_XD_K_B,      4,  "requires EVEX_B, EVEX_K, W, and XD prefix")     \
ENUM_ENTRY(IC_EVEX_W_OPSIZE_K_B,  4,  "requires EVEX_B, EVEX_K, W, and OpSize")        \
ENUM_ENTRY(IC_EVEX_L_K_B,         3,  "requires EVEX_B, EVEX_K and the L prefix")       \
ENUM_ENTRY(IC_EVEX_L_XS_K_B,      4,  "requires EVEX_B, EVEX_K and the L and XS prefix")\
ENUM_ENTRY(IC_EVEX_L_XD_K_B,      4,  "requires EVEX_B, EVEX_K and the L and XD prefix")\
ENUM_ENTRY(IC_EVEX_L_OPSIZE_K_B,  4,  "requires EVEX_B, EVEX_K, L, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L_W_K_B,       3,  "requires EVEX_B, EVEX_K, L and W")               \
ENUM_ENTRY(IC_EVEX_L_W_XS_K_B,    4,  "requires EVEX_B, EVEX_K, L, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_XD_K_B,    4,  "requires EVEX_B, EVEX_K, L, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_OPSIZE_K_B,4,  "requires EVEX_B, EVEX_K, L, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_L2_K_B,        3,  "requires EVEX_B, EVEX_K and the L2 prefix")       \
ENUM_ENTRY(IC_EVEX_L2_XS_K_B,     4,  "requires EVEX_B, EVEX_K and the L2 and XS prefix")\
ENUM_ENTRY(IC_EVEX_L2_XD_K_B,     4,  "requires EVEX_B, EVEX_K and the L2 and XD prefix")\
ENUM_ENTRY(IC_EVEX_L2_OPSIZE_K_B, 4,  "requires EVEX_B, EVEX_K, L2, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L2_W_K_B,      3,  "requires EVEX_B, EVEX_K, L2 and W")               \
ENUM_ENTRY(IC_EVEX_L2_W_XS_K_B,   4,  "requires EVEX_B, EVEX_K, L2, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_XD_K_B,   4,  "requires EVEX_B, EVEX_K, L2, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_OPSIZE_K_B,4,  "requires EVEX_B, EVEX_K, L2, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_KZ_B,           1,  "requires EVEX_B and EVEX_KZ prefix")             \
ENUM_ENTRY(IC_EVEX_XS_KZ_B,        2,  "requires EVEX_B, EVEX_KZ and the XS prefix")     \
ENUM_ENTRY(IC_EVEX_XD_KZ_B,        2,  "requires EVEX_B, EVEX_KZ and the XD prefix")     \
ENUM_ENTRY(IC_EVEX_OPSIZE_KZ_B,    2,  "requires EVEX_B, EVEX_KZ and the OpSize prefix") \
ENUM_ENTRY(IC_EVEX_W_KZ_B,         3,  "requires EVEX_B, EVEX_KZ and the W prefix")      \
ENUM_ENTRY(IC_EVEX_W_XS_KZ_B,      4,  "requires EVEX_B, EVEX_KZ, W, and XS prefix")     \
ENUM_ENTRY(IC_EVEX_W_XD_KZ_B,      4,  "requires EVEX_B, EVEX_KZ, W, and XD prefix")     \
ENUM_ENTRY(IC_EVEX_W_OPSIZE_KZ_B,  4,  "requires EVEX_B, EVEX_KZ, W, and OpSize")        \
ENUM_ENTRY(IC_EVEX_L_KZ_B,           3,  "requires EVEX_B, EVEX_KZ and the L prefix")       \
ENUM_ENTRY(IC_EVEX_L_XS_KZ_B,        4,  "requires EVEX_B, EVEX_KZ and the L and XS prefix")\
ENUM_ENTRY(IC_EVEX_L_XD_KZ_B,        4,  "requires EVEX_B, EVEX_KZ and the L and XD prefix")\
ENUM_ENTRY(IC_EVEX_L_OPSIZE_KZ_B,    4,  "requires EVEX_B, EVEX_KZ, L, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L_W_KZ_B,         3,  "requires EVEX_B, EVEX_KZ, L and W")               \
ENUM_ENTRY(IC_EVEX_L_W_XS_KZ_B,      4,  "requires EVEX_B, EVEX_KZ, L, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_XD_KZ_B,      4,  "requires EVEX_B, EVEX_KZ, L, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_OPSIZE_KZ_B,  4,  "requires EVEX_B, EVEX_KZ, L, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_L2_KZ_B,          3,  "requires EVEX_B, EVEX_KZ and the L2 prefix")       \
ENUM_ENTRY(IC_EVEX_L2_XS_KZ_B,       4,  "requires EVEX_B, EVEX_KZ and the L2 and XS prefix")\
ENUM_ENTRY(IC_EVEX_L2_XD_KZ_B,       4,  "requires EVEX_B, EVEX_KZ and the L2 and XD prefix")\
ENUM_ENTRY(IC_EVEX_L2_OPSIZE_KZ_B,   4,  "requires EVEX_B, EVEX_KZ, L2, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L2_W_KZ_B,        3,  "requires EVEX_B, EVEX_KZ, L2 and W")               \
ENUM_ENTRY(IC_EVEX_L2_W_XS_KZ_B,     4,  "requires EVEX_B, EVEX_KZ, L2, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_XD_KZ_B,     4,  "requires EVEX_B, EVEX_KZ, L2, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_OPSIZE_KZ_B, 4,  "requires EVEX_B, EVEX_KZ, L2, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_KZ,             1,  "requires an EVEX_KZ prefix")             \
ENUM_ENTRY(IC_EVEX_XS_KZ,          2,  "requires EVEX_KZ and the XS prefix")     \
ENUM_ENTRY(IC_EVEX_XD_KZ,          2,  "requires EVEX_KZ and the XD prefix")     \
ENUM_ENTRY(IC_EVEX_OPSIZE_KZ,      2,  "requires EVEX_KZ and the OpSize prefix") \
ENUM_ENTRY(IC_EVEX_W_KZ,           3,  "requires EVEX_KZ and the W prefix")      \
ENUM_ENTRY(IC_EVEX_W_XS_KZ,        4,  "requires EVEX_KZ, W, and XS prefix")     \
ENUM_ENTRY(IC_EVEX_W_XD_KZ,        4,  "requires EVEX_KZ, W, and XD prefix")     \
ENUM_ENTRY(IC_EVEX_W_OPSIZE_KZ,    4,  "requires EVEX_KZ, W, and OpSize")        \
ENUM_ENTRY(IC_EVEX_L_KZ,           3,  "requires EVEX_KZ and the L prefix")       \
ENUM_ENTRY(IC_EVEX_L_XS_KZ,        4,  "requires EVEX_KZ and the L and XS prefix")\
ENUM_ENTRY(IC_EVEX_L_XD_KZ,        4,  "requires EVEX_KZ and the L and XD prefix")\
ENUM_ENTRY(IC_EVEX_L_OPSIZE_KZ,    4,  "requires EVEX_KZ, L, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L_W_KZ,         3,  "requires EVEX_KZ, L and W")               \
ENUM_ENTRY(IC_EVEX_L_W_XS_KZ,      4,  "requires EVEX_KZ, L, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_XD_KZ,      4,  "requires EVEX_KZ, L, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L_W_OPSIZE_KZ,  4,  "requires EVEX_KZ, L, W and OpSize")       \
ENUM_ENTRY(IC_EVEX_L2_KZ,          3,  "requires EVEX_KZ and the L2 prefix")       \
ENUM_ENTRY(IC_EVEX_L2_XS_KZ,       4,  "requires EVEX_KZ and the L2 and XS prefix")\
ENUM_ENTRY(IC_EVEX_L2_XD_KZ,       4,  "requires EVEX_KZ and the L2 and XD prefix")\
ENUM_ENTRY(IC_EVEX_L2_OPSIZE_KZ,   4,  "requires EVEX_KZ, L2, and OpSize")         \
ENUM_ENTRY(IC_EVEX_L2_W_KZ,        3,  "requires EVEX_KZ, L2 and W")               \
ENUM_ENTRY(IC_EVEX_L2_W_XS_KZ,     4,  "requires EVEX_KZ, L2, W and XS prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_XD_KZ,     4,  "requires EVEX_KZ, L2, W and XD prefix")    \
ENUM_ENTRY(IC_EVEX_L2_W_OPSIZE_KZ, 4,  "requires EVEX_KZ, L2, W and OpSize")     

#define ENUM_ENTRY(n, r, d) n,
typedef enum
{
	INSTRUCTION_CONTEXTS
	IC_max
} InstructionContext;
#undef ENUM_ENTRY

/*
* Opcode types, which determine which decode table to use, both in the Intel
* manual and also for the decoder.
*/
typedef enum
{
	ONEBYTE = 0,
	TWOBYTE = 1,
	THREEBYTE_38 = 2,
	THREEBYTE_3A = 3,
	XOP8_MAP = 4,
	XOP9_MAP = 5,
	XOPA_MAP = 6,
	T3DNOW_MAP = 7
} OpcodeType;

/*
* The following structs are used for the hierarchical decode table.  After
* determining the instruction's class (i.e., which IC_* constant applies to
* it), the decoder reads the opcode.  Some instructions require specific
* values of the ModR/M byte, so the ModR/M byte indexes into the final table.
*
* If a ModR/M byte is not required, "required" is left unset, and the values
* for each instructionID are identical.
*/

typedef uint16_t InstrUID;

/*
* ModRMDecisionType - describes the type of ModR/M decision, allowing the
* consumer to determine the number of entries in it.
*
* MODRM_ONEENTRY - No matter what the value of the ModR/M byte is, the decoded
*                  instruction is the same.
* MODRM_SPLITRM  - If the ModR/M byte is between 0x00 and 0xbf, the opcode
*                  corresponds to one instruction; otherwise, it corresponds to
*                  a different instruction.
* MODRM_SPLITMISC- If the ModR/M byte is between 0x00 and 0xbf, ModR/M byte
*                  divided by 8 is used to select instruction; otherwise, each
*                  value of the ModR/M byte could correspond to a different
*                  instruction.
* MODRM_SPLITREG - ModR/M byte divided by 8 is used to select instruction. This
corresponds to instructions that use reg field as opcode
* MODRM_FULL     - Potentially, each value of the ModR/M byte could correspond
*                  to a different instruction.
*/

#define MODRMTYPES            \
	ENUM_ENTRY(MODRM_ONEENTRY)  \
ENUM_ENTRY(MODRM_SPLITRM)   \
ENUM_ENTRY(MODRM_SPLITMISC)  \
ENUM_ENTRY(MODRM_SPLITREG)  \
ENUM_ENTRY(MODRM_FULL)

#define ENUM_ENTRY(n) n,
typedef enum
{
	MODRMTYPES
	MODRM_max
} ModRMDecisionType;
#undef ENUM_ENTRY

#define CASE_ENCODING_RM     \
    case ENCODING_RM:        \
    case ENCODING_RM_CD2:    \
    case ENCODING_RM_CD4:    \
    case ENCODING_RM_CD8:    \
    case ENCODING_RM_CD16:   \
    case ENCODING_RM_CD32:   \
    case ENCODING_RM_CD64

// Physical encodings of instruction operands.

#define ENCODINGS                                                            \
ENUM_ENTRY(ENCODING_NONE,   "")                                              \
ENUM_ENTRY(ENCODING_REG,    "Register operand in ModR/M byte.")              \
ENUM_ENTRY(ENCODING_RM,     "R/M operand in ModR/M byte.")                   \
ENUM_ENTRY(ENCODING_RM_CD2, "R/M operand with CDisp scaling of 2")           \
ENUM_ENTRY(ENCODING_RM_CD4, "R/M operand with CDisp scaling of 4")           \
ENUM_ENTRY(ENCODING_RM_CD8, "R/M operand with CDisp scaling of 8")           \
ENUM_ENTRY(ENCODING_RM_CD16,"R/M operand with CDisp scaling of 16")          \
ENUM_ENTRY(ENCODING_RM_CD32,"R/M operand with CDisp scaling of 32")          \
ENUM_ENTRY(ENCODING_RM_CD64,"R/M operand with CDisp scaling of 64")          \
ENUM_ENTRY(ENCODING_VVVV,   "Register operand in VEX.vvvv byte.")            \
ENUM_ENTRY(ENCODING_WRITEMASK, "Register operand in EVEX.aaa byte.")         \
ENUM_ENTRY(ENCODING_CB,     "1-byte code offset (possible new CS value)")    \
ENUM_ENTRY(ENCODING_CW,     "2-byte")                                        \
ENUM_ENTRY(ENCODING_CD,     "4-byte")                                        \
ENUM_ENTRY(ENCODING_CP,     "6-byte")                                        \
ENUM_ENTRY(ENCODING_CO,     "8-byte")                                        \
ENUM_ENTRY(ENCODING_CT,     "10-byte")                                       \
ENUM_ENTRY(ENCODING_IB,     "1-byte immediate")                              \
ENUM_ENTRY(ENCODING_IW,     "2-byte")                                        \
ENUM_ENTRY(ENCODING_ID,     "4-byte")                                        \
ENUM_ENTRY(ENCODING_IO,     "8-byte")                                        \
ENUM_ENTRY(ENCODING_RB,     "(AL..DIL, R8L..R15L) Register code added to "   \
                            "the opcode byte")                               \
ENUM_ENTRY(ENCODING_RW,     "(AX..DI, R8W..R15W)")                           \
ENUM_ENTRY(ENCODING_RD,     "(EAX..EDI, R8D..R15D)")                         \
ENUM_ENTRY(ENCODING_RO,     "(RAX..RDI, R8..R15)")                           \
ENUM_ENTRY(ENCODING_FP,     "Position on floating-point stack in ModR/M "    \
                            "byte.")                                         \
ENUM_ENTRY(ENCODING_Iv,     "Immediate of operand size")                     \
ENUM_ENTRY(ENCODING_Ia,     "Immediate of address size")                     \
ENUM_ENTRY(ENCODING_Rv,     "Register code of operand size added to the "    \
                            "opcode byte")                                   \
ENUM_ENTRY(ENCODING_DUP,    "Duplicate of another operand; ID is encoded "   \
                            "in type")                                       \
ENUM_ENTRY(ENCODING_SI,     "Source index; encoded in OpSize/Adsize prefix") \
ENUM_ENTRY(ENCODING_DI,     "Destination index; encoded in prefixes")

#define ENUM_ENTRY(n, d) n,
typedef enum
{
	ENCODINGS
	ENCODING_max
} OperandEncoding;
#undef ENUM_ENTRY

/*
* Semantic interpretations of instruction operands.
*/

#define TYPES                                                                  \
ENUM_ENTRY(TYPE_NONE,       "")                                              \
ENUM_ENTRY(TYPE_REL8,       "1-byte immediate address")                      \
ENUM_ENTRY(TYPE_REL16,      "2-byte")                                        \
ENUM_ENTRY(TYPE_REL32,      "4-byte")                                        \
ENUM_ENTRY(TYPE_REL64,      "8-byte")                                        \
ENUM_ENTRY(TYPE_PTR1616,    "2+2-byte segment+offset address")               \
ENUM_ENTRY(TYPE_PTR1632,    "2+4-byte")                                      \
ENUM_ENTRY(TYPE_PTR1664,    "2+8-byte")                                      \
ENUM_ENTRY(TYPE_R8,         "1-byte register operand")                       \
ENUM_ENTRY(TYPE_R16,        "2-byte")                                        \
ENUM_ENTRY(TYPE_R32,        "4-byte")                                        \
ENUM_ENTRY(TYPE_R64,        "8-byte")                                        \
ENUM_ENTRY(TYPE_IMM8,       "1-byte immediate operand")                      \
ENUM_ENTRY(TYPE_IMM16,      "2-byte")                                        \
ENUM_ENTRY(TYPE_IMM32,      "4-byte")                                        \
ENUM_ENTRY(TYPE_IMM64,      "8-byte")                                        \
ENUM_ENTRY(TYPE_IMM3,       "1-byte immediate operand between 0 and 7")      \
ENUM_ENTRY(TYPE_IMM5,       "1-byte immediate operand between 0 and 31")     \
ENUM_ENTRY(TYPE_RM8,        "1-byte register or memory operand")             \
ENUM_ENTRY(TYPE_RM16,       "2-byte")                                        \
ENUM_ENTRY(TYPE_RM32,       "4-byte")                                        \
ENUM_ENTRY(TYPE_RM64,       "8-byte")                                        \
ENUM_ENTRY(TYPE_M,          "Memory operand")                                \
ENUM_ENTRY(TYPE_M8,         "1-byte")                                        \
ENUM_ENTRY(TYPE_M16,        "2-byte")                                        \
ENUM_ENTRY(TYPE_M32,        "4-byte")                                        \
ENUM_ENTRY(TYPE_M64,        "8-byte")                                        \
ENUM_ENTRY(TYPE_LEA,        "Effective address")                             \
ENUM_ENTRY(TYPE_M128,       "16-byte (SSE/SSE2)")                            \
ENUM_ENTRY(TYPE_M256,       "256-byte (AVX)")                                \
ENUM_ENTRY(TYPE_M1616,      "2+2-byte segment+offset address")               \
ENUM_ENTRY(TYPE_M1632,      "2+4-byte")                                      \
ENUM_ENTRY(TYPE_M1664,      "2+8-byte")                                      \
ENUM_ENTRY(TYPE_M16_32,     "2+4-byte two-part memory operand (LIDT, LGDT)") \
ENUM_ENTRY(TYPE_M16_16,     "2+2-byte (BOUND)")                              \
ENUM_ENTRY(TYPE_M32_32,     "4+4-byte (BOUND)")                              \
ENUM_ENTRY(TYPE_M16_64,     "2+8-byte (LIDT, LGDT)")                         \
ENUM_ENTRY(TYPE_SRCIDX8,    "1-byte memory at source index")                 \
ENUM_ENTRY(TYPE_SRCIDX16,   "2-byte memory at source index")                 \
ENUM_ENTRY(TYPE_SRCIDX32,   "4-byte memory at source index")                 \
ENUM_ENTRY(TYPE_SRCIDX64,   "8-byte memory at source index")                 \
ENUM_ENTRY(TYPE_DSTIDX8,    "1-byte memory at destination index")            \
ENUM_ENTRY(TYPE_DSTIDX16,   "2-byte memory at destination index")            \
ENUM_ENTRY(TYPE_DSTIDX32,   "4-byte memory at destination index")            \
ENUM_ENTRY(TYPE_DSTIDX64,   "8-byte memory at destination index")            \
ENUM_ENTRY(TYPE_MOFFS8,     "1-byte memory offset (relative to segment "     \
                            "base)")                                         \
ENUM_ENTRY(TYPE_MOFFS16,    "2-byte")                                        \
ENUM_ENTRY(TYPE_MOFFS32,    "4-byte")                                        \
ENUM_ENTRY(TYPE_MOFFS64,    "8-byte")                                        \
ENUM_ENTRY(TYPE_SREG,       "Byte with single bit set: 0 = ES, 1 = CS, "     \
		"2 = SS, 3 = DS, 4 = FS, 5 = GS")                \
ENUM_ENTRY(TYPE_M32FP,      "32-bit IEE754 memory floating-point operand")   \
ENUM_ENTRY(TYPE_M64FP,      "64-bit")                                        \
ENUM_ENTRY(TYPE_M80FP,      "80-bit extended")                               \
ENUM_ENTRY(TYPE_M16INT,     "2-byte memory integer operand for use in "      \
		"floating-point instructions")                   \
ENUM_ENTRY(TYPE_M32INT,     "4-byte")                                        \
ENUM_ENTRY(TYPE_M64INT,     "8-byte")                                        \
ENUM_ENTRY(TYPE_ST,         "Position on the floating-point stack")          \
ENUM_ENTRY(TYPE_MM,         "MMX register operand")                          \
ENUM_ENTRY(TYPE_MM32,       "4-byte MMX register or memory operand")         \
ENUM_ENTRY(TYPE_MM64,       "8-byte")                                        \
ENUM_ENTRY(TYPE_XMM,        "XMM register operand")                          \
ENUM_ENTRY(TYPE_XMM32,      "4-byte XMM register or memory operand")         \
ENUM_ENTRY(TYPE_XMM64,      "8-byte")                                        \
ENUM_ENTRY(TYPE_XMM128,     "16-byte")                                       \
ENUM_ENTRY(TYPE_XMM256,     "32-byte")                                       \
ENUM_ENTRY(TYPE_XMM512,     "64-byte")                                       \
ENUM_ENTRY(TYPE_VK1,        "1-bit")                                         \
ENUM_ENTRY(TYPE_VK2,        "2-bit")                                         \
ENUM_ENTRY(TYPE_VK4,        "4-bit")                                         \
ENUM_ENTRY(TYPE_VK8,        "8-bit")                                         \
ENUM_ENTRY(TYPE_VK16,       "16-bit")                                        \
ENUM_ENTRY(TYPE_VK32,       "32-bit")                                        \
ENUM_ENTRY(TYPE_VK64,       "64-bit")                                        \
ENUM_ENTRY(TYPE_XMM0,       "Implicit use of XMM0")                          \
ENUM_ENTRY(TYPE_SEGMENTREG, "Segment register operand")                      \
ENUM_ENTRY(TYPE_DEBUGREG,   "Debug register operand")                        \
ENUM_ENTRY(TYPE_CONTROLREG, "Control register operand")                      \
\
ENUM_ENTRY(TYPE_Mv,         "Memory operand of operand size")                \
ENUM_ENTRY(TYPE_Rv,         "Register operand of operand size")              \
ENUM_ENTRY(TYPE_IMMv,       "Immediate operand of operand size")             \
ENUM_ENTRY(TYPE_RELv,       "Immediate address of operand size")             \
ENUM_ENTRY(TYPE_DUP0,       "Duplicate of operand 0")                        \
ENUM_ENTRY(TYPE_DUP1,       "operand 1")                                     \
ENUM_ENTRY(TYPE_DUP2,       "operand 2")                                     \
ENUM_ENTRY(TYPE_DUP3,       "operand 3")                                     \
ENUM_ENTRY(TYPE_DUP4,       "operand 4")                                     \
ENUM_ENTRY(TYPE_M512,       "512-bit FPU/MMX/XMM/MXCSR state")

#define ENUM_ENTRY(n, d) n,
typedef enum
{
	TYPES
	TYPE_max
} OperandType;
#undef ENUM_ENTRY

/*
* OperandSpecifier - The specification for how to extract and interpret one
*   operand.
*/
typedef struct OperandSpecifier
{
	uint8_t encoding;
	uint8_t type;
} OperandSpecifier;

/*
* Indicates where the opcode modifier (if any) is to be found.  Extended
* opcodes with AddRegFrm have the opcode modifier in the ModR/M byte.
*/

#define MODIFIER_TYPES        \
	ENUM_ENTRY(MODIFIER_NONE)

#define ENUM_ENTRY(n) n,
typedef enum
{
	MODIFIER_TYPES
	MODIFIER_max
} ModifierType;
#undef ENUM_ENTRY

#define X86_MAX_OPERANDS 5

/*
* Decoding mode for the Intel disassembler.  16-bit, 32-bit, and 64-bit mode
* are supported, and represent real mode, IA-32e, and IA-32e in 64-bit mode,
* respectively.
*/
typedef enum
{
	MODE_16BIT,
	MODE_32BIT,
	MODE_64BIT
} DisassemblerMode;

#endif