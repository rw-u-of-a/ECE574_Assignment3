//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 03-Dec-2017 14:20:47
//

//***********************************************************************
// This automatically generated example C main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "FORCE_DIRECTED_dot_cpp.h"
#include "main.h"
#include "FORCE_DIRECTED_dot_cpp_terminate.h"
#include "FORCE_DIRECTED_dot_cpp_emxAPI.h"
#include "FORCE_DIRECTED_dot_cpp_initialize.h"

// Function Declarations
static emxArray_boolean_T *argInit_1xUnbounded_boolean_T();
static emxArray_uint16_T *argInit_1xUnbounded_uint16_T();
static boolean_T argInit_boolean_T();
static struct0_T argInit_struct0_T();
static unsigned short argInit_uint16_T();
static emxArray_uint16_T *c_argInit_UnboundedxUnbounded_u();
static void main_FORCE_DIRECTED_dot_cpp();

// Function Definitions

//
// Arguments    : void
// Return Type  : emxArray_boolean_T *
//
static emxArray_boolean_T *argInit_1xUnbounded_boolean_T()
{
  emxArray_boolean_T *result;
  static int iv2[2] = { 1, 2 };

  int idx1;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_boolean_T(2, *(int (*)[2])&iv2[0]);

  // Loop over the array to initialize each element.
  for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result->data[result->size[0] * idx1] = argInit_boolean_T();
  }

  return result;
}

//
// Arguments    : void
// Return Type  : emxArray_uint16_T *
//
static emxArray_uint16_T *argInit_1xUnbounded_uint16_T()
{
  emxArray_uint16_T *result;
  static int iv0[2] = { 1, 2 };

  int idx1;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_uint16_T(2, *(int (*)[2])&iv0[0]);

  // Loop over the array to initialize each element.
  for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result->data[result->size[0] * idx1] = argInit_uint16_T();
  }

  return result;
}

//
// Arguments    : void
// Return Type  : boolean_T
//
static boolean_T argInit_boolean_T()
{
  return false;
}

//
// Arguments    : void
// Return Type  : struct0_T
//
static struct0_T argInit_struct0_T()
{
  struct0_T result;

  // Set the value of each structure field.
  // Change this value to the value that the application requires.
  result.numbers = argInit_1xUnbounded_uint16_T();
  result.children = c_argInit_UnboundedxUnbounded_u();
  result.parents = c_argInit_UnboundedxUnbounded_u();
  result.resource = argInit_1xUnbounded_uint16_T();
  result.scheduled = argInit_1xUnbounded_boolean_T();
  return result;
}

//
// Arguments    : void
// Return Type  : unsigned short
//
static unsigned short argInit_uint16_T()
{
  return 0;
}

//
// Arguments    : void
// Return Type  : emxArray_uint16_T *
//
static emxArray_uint16_T *c_argInit_UnboundedxUnbounded_u()
{
  emxArray_uint16_T *result;
  static int iv1[2] = { 2, 2 };

  int idx0;
  int idx1;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_uint16_T(2, *(int (*)[2])&iv1[0]);

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result->data[idx0 + result->size[0] * idx1] = argInit_uint16_T();
    }
  }

  return result;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_FORCE_DIRECTED_dot_cpp()
{
  emxArray_real_T *OUTPUT_SCHEDULE;
  struct0_T operations;
  emxInitArray_real_T(&OUTPUT_SCHEDULE, 2);

  // Initialize function 'FORCE_DIRECTED_dot_cpp' input arguments.
  // Initialize function input argument 'operations'.
  operations = argInit_struct0_T();

  // Call the entry-point 'FORCE_DIRECTED_dot_cpp'.
  FORCE_DIRECTED_dot_cpp(&operations, argInit_uint16_T(), OUTPUT_SCHEDULE);
  emxDestroyArray_real_T(OUTPUT_SCHEDULE);
  emxDestroy_struct0_T(operations);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  FORCE_DIRECTED_dot_cpp_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_FORCE_DIRECTED_dot_cpp();

  // Terminate the application.
  // You do not need to do this more than one time.
  FORCE_DIRECTED_dot_cpp_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//
