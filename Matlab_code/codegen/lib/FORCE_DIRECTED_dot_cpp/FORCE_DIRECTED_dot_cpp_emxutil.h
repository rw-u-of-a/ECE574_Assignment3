//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: FORCE_DIRECTED_dot_cpp_emxutil.h
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 03-Dec-2017 14:20:47
//
#ifndef FORCE_DIRECTED_DOT_CPP_EMXUTIL_H
#define FORCE_DIRECTED_DOT_CPP_EMXUTIL_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "FORCE_DIRECTED_dot_cpp_types.h"

// Function Declarations
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
extern void emxFreeStruct_struct0_T(struct0_T *pStruct);
extern void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
extern void emxFree_int32_T(emxArray_int32_T **pEmxArray);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxFree_uint16_T(emxArray_uint16_T **pEmxArray);
extern void emxInitStruct_struct0_T(struct0_T *pStruct);
extern void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions);
extern void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
extern void emxInit_int32_T1(emxArray_int32_T **pEmxArray, int numDimensions);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxInit_uint16_T(emxArray_uint16_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for FORCE_DIRECTED_dot_cpp_emxutil.h
//
// [EOF]
//
