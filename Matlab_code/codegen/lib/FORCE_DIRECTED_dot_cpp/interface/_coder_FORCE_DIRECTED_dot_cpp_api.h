/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_FORCE_DIRECTED_dot_cpp_api.h
 *
 * MATLAB Coder version            : 3.2
 * C/C++ source code generated on  : 03-Dec-2017 14:20:47
 */

#ifndef _CODER_FORCE_DIRECTED_DOT_CPP_API_H
#define _CODER_FORCE_DIRECTED_DOT_CPP_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_FORCE_DIRECTED_dot_cpp_api.h"

/* Type Definitions */
#ifndef struct_emxArray_boolean_T
#define struct_emxArray_boolean_T

struct emxArray_boolean_T
{
  boolean_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_boolean_T*/

#ifndef typedef_emxArray_boolean_T
#define typedef_emxArray_boolean_T

typedef struct emxArray_boolean_T emxArray_boolean_T;

#endif                                 /*typedef_emxArray_boolean_T*/

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

#ifndef struct_emxArray_uint16_T
#define struct_emxArray_uint16_T

struct emxArray_uint16_T
{
  uint16_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_uint16_T*/

#ifndef typedef_emxArray_uint16_T
#define typedef_emxArray_uint16_T

typedef struct emxArray_uint16_T emxArray_uint16_T;

#endif                                 /*typedef_emxArray_uint16_T*/

#ifndef typedef_struct0_T
#define typedef_struct0_T

typedef struct {
  emxArray_uint16_T *numbers;
  emxArray_uint16_T *children;
  emxArray_uint16_T *parents;
  emxArray_uint16_T *resource;
  emxArray_boolean_T *scheduled;
} struct0_T;

#endif                                 /*typedef_struct0_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void FORCE_DIRECTED_dot_cpp(struct0_T *operations, uint16_T Latency,
  emxArray_real_T *OUTPUT_SCHEDULE);
extern void FORCE_DIRECTED_dot_cpp_api(const mxArray * const prhs[2], const
  mxArray *plhs[1]);
extern void FORCE_DIRECTED_dot_cpp_atexit(void);
extern void FORCE_DIRECTED_dot_cpp_initialize(void);
extern void FORCE_DIRECTED_dot_cpp_terminate(void);
extern void FORCE_DIRECTED_dot_cpp_xil_terminate(void);

#endif

/*
 * File trailer for _coder_FORCE_DIRECTED_dot_cpp_api.h
 *
 * [EOF]
 */
