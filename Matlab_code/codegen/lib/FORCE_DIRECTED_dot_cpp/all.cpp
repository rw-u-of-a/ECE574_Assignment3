//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: all.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 03-Dec-2017 14:20:47
//

// Include Files
#include "rt_nonfinite.h"
#include "FORCE_DIRECTED_dot_cpp.h"
#include "all.h"

// Function Definitions

//
// Arguments    : const emxArray_boolean_T *x
// Return Type  : boolean_T
//
boolean_T all(const emxArray_boolean_T *x)
{
  boolean_T y;
  int ix;
  boolean_T exitg1;
  y = true;
  ix = 1;
  exitg1 = false;
  while ((!exitg1) && (ix <= x->size[1])) {
    if (!x->data[ix - 1]) {
      y = false;
      exitg1 = true;
    } else {
      ix++;
    }
  }

  return y;
}

//
// File trailer for all.cpp
//
// [EOF]
//
