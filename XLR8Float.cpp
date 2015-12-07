/*--------------------------------------------------------------------
 Copyright (c) 2015 Alorim Technology.  All right reserved.
 This file is part of the Alorium Technology XLR8 Floating Point library.
 Written by Matt Weber (Matthew.D.Weber@ieee.org) of
   Alorium Technology (info@aloriumtech.com)
 Uses the floating point hardware in Alorium Technology's XLR8 board
  (which happens to be otherwise Arduino Uno compatible) to significantly
  accelerate some floating point functions.

 
 
 XLR8 NeoPixel is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation, either version 3 of
 the License, or (at your option) any later version.
 
 XLR8 NeoPixel is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with XLR8 NeoPixel.  If not, see
 <http://www.gnu.org/licenses/>.
 --------------------------------------------------------------------*/

#include "XLR8Float.h"

#  define XLR8_FP_CTRL   _SFR_IO8(0x10)
#  define XLR8_FP_STATUS _SFR_IO8(0x11)
#  define XLR8_FP_R0     _SFR_IO8(0x0C)
#  define XLR8_FP_R1     _SFR_IO8(0x0D)
#  define XLR8_FP_R2     _SFR_IO8(0x0E)
#  define XLR8_FP_R3     _SFR_IO8(0x0F)
#  define XLR8_FP_R01    _SFR_IO16(0x0C)
#  define XLR8_FP_R23    _SFR_IO16(0x0E)

// other control register bits
#define XLR8_FP_GO_BIT 6
// control register command value definitions for bits 4:0
#define XLR8_FP_ADD_CMD  (1 | (1 << XLR8_FP_GO_BIT))
#define XLR8_FP_SUB_CMD  (2 | (1 << XLR8_FP_GO_BIT))
#define XLR8_FP_MULT_CMD (3 | (1 << XLR8_FP_GO_BIT))
#define XLR8_FP_DIV_CMD  (4 | (1 << XLR8_FP_GO_BIT))
// status register bit positions
#define XLR8_FP_DONE_BIT 7
#define XLR8_FP_ERROR_BIT 6

#define INVERTSIGN() \
  __asm__ __volatile__ ( \
  "subi	r21, 0x80" "\n\t" \
  :: \
 );

#define SENDXLR8CMD(val) \
    __asm__ __volatile__ ( \
        "ldi r26, %[cmd]" "\n\t" \
        "out %[ctrl], r26" "\n\t" \
        ::[ctrl] "I" (_SFR_IO_ADDR(XLR8_FP_CTRL)),[cmd] "M" (val) \
    );
#define COPYXLR8RESULT() \
    __asm__ __volatile__ ( \
    "in r22, %[rslt0]" "\n\t" \
    "in r23, %[rslt1]" "\n\t" \
    "in r24, %[rslt2]" "\n\t" \
    "in r25, %[rslt3]" "\n\t" \
    ::[rslt0] "I" (_SFR_IO_ADDR(XLR8_FP_R0)), \
      [rslt1] "I" (_SFR_IO_ADDR(XLR8_FP_R1)), \
      [rslt2] "I" (_SFR_IO_ADDR(XLR8_FP_R2)), \
      [rslt3] "I" (_SFR_IO_ADDR(XLR8_FP_R3))  \
);

float __attribute__ ((noinline)) xlr8FloatAdd (float opa, float opb) {
    // Becase operands were loaded before this function call, the
    //  results are already done and we can immediately load
    //  the result reg without waiting for the done status to set.
    // Using assembly instead of C code to copy results because the compiled C
    //  code ends up brining XLR8_FP_R* into r19:18 and then copying from
    //  there to the output registers r25:22. Also, the compiler thinks
    //  the data being copied from XLR8_FP_R* is integer data and we would
    //  need to use a union or other trick to get it viewed as float for
    //  the function return value
    SENDXLR8CMD(XLR8_FP_ADD_CMD)
    COPYXLR8RESULT()
}
float __attribute__ ((noinline)) xlr8FloatSub (float opa, float opb) {
  INVERTSIGN()  
    // Give time for updated operand to propogate through function
    __asm__ __volatile__ ("nop"  "\n\t" ::);
    __asm__ __volatile__ ("nop"  "\n\t" ::);
    SENDXLR8CMD(XLR8_FP_ADD_CMD)
    COPYXLR8RESULT()
}
float __attribute__ ((noinline)) xlr8FloatMult (float opa, float opb) {
    SENDXLR8CMD(XLR8_FP_MULT_CMD)
    COPYXLR8RESULT()
}
float __attribute__ ((noinline)) xlr8FloatDiv (float opa, float opb) {
    SENDXLR8CMD(XLR8_FP_DIV_CMD)
    while (!(XLR8_FP_STATUS & _BV(XLR8_FP_DONE_BIT))); // div is slower, hang here until we're done
    if (XLR8_FP_STATUS & _BV(XLR8_FP_ERROR_BIT)) {
      return opa / opb; // if no div unit, do it the slow old fashioned way
    } else {
      COPYXLR8RESULT()
    }
}
