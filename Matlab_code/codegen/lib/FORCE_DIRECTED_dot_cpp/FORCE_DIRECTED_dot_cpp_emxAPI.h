//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: FORCE_DIRECTED_dot_cpp_emxAPI.h
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 03-Dec-2017 14:20:47
//
#ifndef FORCE_DIRECTED_DOT_CPP_EMXAPI_H
#define FORCE_DIRECTED_DOT_CPP_EMXAPI_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "FORCE_DIRECTED_dot_cpp_types.h"

// Function Declarations
extern emxArray_boolean_T *emxCreateND_boolean_T(int numDimensions, int *size);
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size);
extern emxArray_uint16_T *emxCreateND_uint16_T(int numDimensions, int *size);
extern emxArray_boolean_T *emxCreateWrapperND_boolean_T(boolean_T *data, int
  numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, int *size);
extern emxArray_uint16_T *emxCreateWrapperND_uint16_T(unsigned short *data, int
  numDimensions, int *size);
extern emxArray_boolean_T *emxCreateWrapper_boolean_T(boolean_T *data, int rows,
  int cols);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_uint16_T *emxCreateWrapper_uint16_T(unsigned short *data, int
  rows, int cols);
extern emxArray_boolean_T *emxCreate_boolean_T(int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern emxArray_uint16_T *emxCreate_uint16_T(int rows, int cols);
extern void emxDestroyArray_boolean_T(emxArray_boolean_T *emxArray);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void emxDestroyArray_uint16_T(emxArray_uint16_T *emxArray);
extern void emxDestroy_struct0_T(struct0_T emxArray);
extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxInit_struct0_T(struct0_T *pStruct);

#endif

//
// File trailer for FORCE_DIRECTED_dot_cpp_emxAPI.h
//
// [EOF]
//
