/* xlr8_float_simple
 *  
 *  Copyright (c) 2021 Alorium Technology
 *  Bryan Craker (support@aloriumtech.com)
 *  
 *  Demonstrates how to use XLR8Float functions compared to normal software 
 *  float functions.
 */

#include <XLR8Float.h>

float a, b; // Normal software float operands
float c;    // Normal software float result

// Declare variables used with the FPGA to be volatile, otherwise values 
// may change in hardware without software updating memory.
volatile float d, e; // XLR8 float operands
volatile float f;    // XLR8 float result

void setup() {

  Serial.begin(115200);

  a = 3.14;
  b = 1.62;

  d = 3.14;
  e = 1.62;

  Serial.println("Simple XLR8 Float Example");
  Serial.println();

  // Add
  c = a + b;
  f = xlr8FloatAdd(d, e);
  Serial.println("Floating Point Addition");
  Serial.print("  Software Result: ");
  Serial.println(c);
  Serial.print("  FPGA Result: ");
  Serial.println(f);
  Serial.println();

  // Subtract
  c = a - b;
  f = xlr8FloatSub(d, e);
  Serial.println("Floating Point Subtraction");
  Serial.print("  Software Result: ");
  Serial.println(c);
  Serial.print("  FPGA Result: ");
  Serial.println(f);
  Serial.println();

  // Multiply
  c = a * b;
  f = xlr8FloatMult(d, e);
  Serial.println("Floating Point Multiplication");
  Serial.print("  Software Result: ");
  Serial.println(c);
  Serial.print("  FPGA Result: ");
  Serial.println(f);
  Serial.println();

  // Divide
  c = a / b;
  f = xlr8FloatDiv(d, e);
  Serial.println("Floating Point Division");
  Serial.print("  Software Result: ");
  Serial.println(c);
  Serial.print("  FPGA Result: ");
  Serial.println(f);
  Serial.println();

}

void loop() {
  // Do nothing
}
