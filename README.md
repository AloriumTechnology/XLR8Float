# XLR8 Floating Point Library
Floating point math on Arduino processors is slow.  
Floating point math on Alorium Technology's XLR8 Floating Point Library is fast.

**Update as of version 2.0.0**
The Arduino GCC compiler has made optimizations that unfortunately impact FPGA access to the operands. To ensure the compiler does not interfere with XLR8Float processing, please declare your operand and result variables as "volatile" in your sketches.

**Features:**

- Arduino Uno compatible  
- Floating Point Add, Subtract, and Multiply in just 9 cycles (versus ~100-130 on AVR processors)
- Floating Point Divide in just 17 cycles (versus 500-600 on AVR processors)  

**Usage:**

- Select Uno board in Arduino IDE  
- Include XLR8Float Library  
- Use accelerated functions for floating point math  
  c = xlr8FloatAdd(a,b);    // performs c = a + b  
  c = xlr8FloatSub(a,b);    // performs c = a - b  
  c = xlr8FloatMult(a,b);   // performs c = a * b  
  c = xlr8FloatDiv(a,b);    // performs c = a / b  
