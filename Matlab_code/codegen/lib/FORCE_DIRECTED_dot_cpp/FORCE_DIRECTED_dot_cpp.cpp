//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: FORCE_DIRECTED_dot_cpp.cpp
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 03-Dec-2017 14:20:47
//

// Include Files
#include "rt_nonfinite.h"
#include "FORCE_DIRECTED_dot_cpp.h"
#include "FORCE_DIRECTED_dot_cpp_emxutil.h"
#include "all.h"

// Function Declarations
static void count_children(unsigned short v, double *child_count, const
  emxArray_uint16_T *operations_children, const emxArray_uint16_T
  *operations_resource);
static void count_parents(unsigned short v, double *parent_count, const
  emxArray_uint16_T *operations_parents, const emxArray_uint16_T
  *operations_resource);
static double rt_roundd_snf(double u);

// Function Definitions

//
// Arguments    : unsigned short v
//                double *child_count
//                const emxArray_uint16_T *operations_children
//                const emxArray_uint16_T *operations_resource
// Return Type  : void
//
static void count_children(unsigned short v, double *child_count, const
  emxArray_uint16_T *operations_children, const emxArray_uint16_T
  *operations_resource)
{
  emxArray_uint16_T *local_family;
  int loop_ub;
  int i1;
  emxArray_boolean_T *b_local_family;
  emxArray_real_T *local_child_count;
  double mtmp;
  emxInit_uint16_T(&local_family, 2);
  loop_ub = operations_children->size[1];
  i1 = local_family->size[0] * local_family->size[1];
  local_family->size[0] = 1;
  local_family->size[1] = loop_ub;
  emxEnsureCapacity((emxArray__common *)local_family, i1, (int)sizeof(unsigned
    short));
  for (i1 = 0; i1 < loop_ub; i1++) {
    local_family->data[local_family->size[0] * i1] = operations_children->data
      [(v + operations_children->size[0] * i1) - 1];
  }

  emxInit_boolean_T(&b_local_family, 2);
  i1 = b_local_family->size[0] * b_local_family->size[1];
  b_local_family->size[0] = 1;
  b_local_family->size[1] = local_family->size[1];
  emxEnsureCapacity((emxArray__common *)b_local_family, i1, (int)sizeof
                    (boolean_T));
  loop_ub = local_family->size[0] * local_family->size[1];
  for (i1 = 0; i1 < loop_ub; i1++) {
    b_local_family->data[i1] = false;
  }

  if (!all(b_local_family)) {
    emxInit_real_T1(&local_child_count, 1);
    loop_ub = operations_children->size[1];
    i1 = local_child_count->size[0];
    local_child_count->size[0] = loop_ub;
    emxEnsureCapacity((emxArray__common *)local_child_count, i1, (int)sizeof
                      (double));
    for (i1 = 0; i1 < loop_ub; i1++) {
      local_child_count->data[i1] = *child_count;
    }

    i1 = operations_children->size[1];
    for (loop_ub = 0; loop_ub < i1; loop_ub++) {
      local_child_count->data[local_family->data[loop_ub] - 1] += 1.0 + ((double)
        operations_resource->data[v - 1] - 1.0);
      count_children(local_family->data[loop_ub], &local_child_count->
                     data[local_family->data[loop_ub] - 1], operations_children,
                     operations_resource);
    }

    mtmp = local_child_count->data[0];
    if (local_child_count->size[0] > 1) {
      for (loop_ub = 1; loop_ub + 1 <= local_child_count->size[0]; loop_ub++) {
        if (local_child_count->data[loop_ub] > mtmp) {
          mtmp = local_child_count->data[loop_ub];
        }
      }
    }

    emxFree_real_T(&local_child_count);
    *child_count = mtmp;
  } else {
    // only has "out" as child
  }

  emxFree_boolean_T(&b_local_family);
  emxFree_uint16_T(&local_family);
}

//
// Arguments    : unsigned short v
//                double *parent_count
//                const emxArray_uint16_T *operations_parents
//                const emxArray_uint16_T *operations_resource
// Return Type  : void
//
static void count_parents(unsigned short v, double *parent_count, const
  emxArray_uint16_T *operations_parents, const emxArray_uint16_T
  *operations_resource)
{
  emxArray_uint16_T *local_family;
  int loop_ub;
  int i0;
  double y;
  int exitg1;
  emxArray_real_T *local_parent_count;
  emxInit_uint16_T(&local_family, 2);

  //  Other Functions
  loop_ub = operations_parents->size[1];
  i0 = local_family->size[0] * local_family->size[1];
  local_family->size[0] = 1;
  local_family->size[1] = loop_ub;
  emxEnsureCapacity((emxArray__common *)local_family, i0, (int)sizeof(unsigned
    short));
  for (i0 = 0; i0 < loop_ub; i0++) {
    local_family->data[local_family->size[0] * i0] = operations_parents->data[(v
      + operations_parents->size[0] * i0) - 1];
  }

  i0 = operations_parents->size[1];
  if (i0 == 0) {
    y = 0.0;
  } else {
    y = operations_parents->data[v - 1];
    loop_ub = 2;
    do {
      exitg1 = 0;
      i0 = operations_parents->size[1];
      if (loop_ub <= i0) {
        y += (double)local_family->data[loop_ub - 1];
        loop_ub++;
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  if (!(y == 0.0)) {
    emxInit_real_T1(&local_parent_count, 1);
    loop_ub = operations_parents->size[1];
    i0 = local_parent_count->size[0];
    local_parent_count->size[0] = loop_ub;
    emxEnsureCapacity((emxArray__common *)local_parent_count, i0, (int)sizeof
                      (double));
    for (i0 = 0; i0 < loop_ub; i0++) {
      local_parent_count->data[i0] = *parent_count;
    }

    i0 = operations_parents->size[1];
    for (loop_ub = 0; loop_ub < i0; loop_ub++) {
      local_parent_count->data[local_family->data[loop_ub] - 1] += 1.0 +
        ((double)operations_resource->data[v - 1] - 1.0);
      count_parents(local_family->data[loop_ub], &local_parent_count->
                    data[local_family->data[loop_ub] - 1], operations_parents,
                    operations_resource);
    }

    y = local_parent_count->data[0];
    if (local_parent_count->size[0] > 1) {
      for (loop_ub = 1; loop_ub + 1 <= local_parent_count->size[0]; loop_ub++) {
        if (local_parent_count->data[loop_ub] > y) {
          y = local_parent_count->data[loop_ub];
        }
      }
    }

    emxFree_real_T(&local_parent_count);
    *parent_count = y;
  } else {
    // only has "in" as parent
  }

  emxFree_uint16_T(&local_family);
}

//
// Arguments    : double u
// Return Type  : double
//
static double rt_roundd_snf(double u)
{
  double y;
  if (std::abs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = std::floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = std::ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

//
// Arguments    : struct0_T *operations
//                unsigned short Latency
//                emxArray_real_T *OUTPUT_SCHEDULE
// Return Type  : void
//
void FORCE_DIRECTED_dot_cpp(struct0_T *operations, unsigned short Latency,
  emxArray_real_T *OUTPUT_SCHEDULE)
{
  boolean_T all_operations_scheduled;
  int kEnd;
  int i2;
  emxArray_real_T *Time_Frames;
  emxArray_real_T *parent_count;
  int qEnd;
  emxArray_uint16_T *b_operations;
  emxArray_uint16_T *c_operations;
  emxArray_uint16_T *d_operations;
  emxArray_uint16_T *e_operations;
  emxArray_real_T *Operation_Probability;
  emxArray_real_T *Type_Probability;
  emxArray_real_T *Self_Force;
  emxArray_real_T *time_options;
  emxArray_int32_T *min_force_cycle;
  emxArray_uint16_T *b;
  emxArray_int32_T *idx;
  emxArray_int32_T *iwork;
  emxArray_real_T *minval;
  emxArray_boolean_T *x;
  emxArray_int32_T *jj;
  double ndbl;
  unsigned short b_x;
  int na;
  int n;
  int j;
  int k;
  int i;
  int pEnd;
  int p;
  int q;
  unsigned short c;
  double apnd;
  double cdiff;
  double absa;
  double absb;
  boolean_T exitg3;
  boolean_T exitg4;
  boolean_T exitg2;
  boolean_T guard1 = false;
  boolean_T exitg1;

  //  input nop
  //  output nop
  all_operations_scheduled = false;

  // init output
  //  the output of this function is a cell array where each cell represents
  //  a time cycle, and the contents of the cell is an array of all the
  //  nodes that should be scheduled in that cycle
  //  OUTPUT_SCHEDULE = cell(Latency,1);
  kEnd = OUTPUT_SCHEDULE->size[0] * OUTPUT_SCHEDULE->size[1];
  OUTPUT_SCHEDULE->size[0] = operations->numbers->size[1];
  OUTPUT_SCHEDULE->size[1] = operations->numbers->size[1];
  emxEnsureCapacity((emxArray__common *)OUTPUT_SCHEDULE, kEnd, (int)sizeof
                    (double));
  i2 = operations->numbers->size[1] * operations->numbers->size[1];
  for (kEnd = 0; kEnd < i2; kEnd++) {
    OUTPUT_SCHEDULE->data[kEnd] = 0.0;
  }

  emxInit_real_T(&Time_Frames, 2);

  //  ALU is resource 1 with latency of 1 cycle
  //  MULT is resource 2 with latency of 2 cycles
  //  DIV/MOD is resource 3 with latency of 3 cycles
  //  Compute the time frames
  kEnd = Time_Frames->size[0] * Time_Frames->size[1];
  Time_Frames->size[0] = operations->numbers->size[1];
  Time_Frames->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)Time_Frames, kEnd, (int)sizeof(double));
  i2 = operations->numbers->size[1] << 1;
  for (kEnd = 0; kEnd < i2; kEnd++) {
    Time_Frames->data[kEnd] = 0.0;
  }

  emxInit_real_T1(&parent_count, 1);

  // ASAP Schedule
  //  ASAP = cell(Latency,1);
  kEnd = parent_count->size[0];
  parent_count->size[0] = operations->numbers->size[1];
  emxEnsureCapacity((emxArray__common *)parent_count, kEnd, (int)sizeof(double));
  i2 = operations->numbers->size[1];
  for (kEnd = 0; kEnd < i2; kEnd++) {
    parent_count->data[kEnd] = 0.0;
  }

  //  loop through each node
  qEnd = 0;
  emxInit_uint16_T(&b_operations, 2);
  emxInit_uint16_T(&c_operations, 2);
  while (qEnd <= operations->numbers->size[1] - 1) {
    //  find how many generations of parents this node has
    kEnd = b_operations->size[0] * b_operations->size[1];
    b_operations->size[0] = operations->parents->size[0];
    b_operations->size[1] = operations->parents->size[1];
    emxEnsureCapacity((emxArray__common *)b_operations, kEnd, (int)sizeof
                      (unsigned short));
    i2 = operations->parents->size[0] * operations->parents->size[1];
    for (kEnd = 0; kEnd < i2; kEnd++) {
      b_operations->data[kEnd] = operations->parents->data[kEnd];
    }

    kEnd = c_operations->size[0] * c_operations->size[1];
    c_operations->size[0] = 1;
    c_operations->size[1] = operations->resource->size[1];
    emxEnsureCapacity((emxArray__common *)c_operations, kEnd, (int)sizeof
                      (unsigned short));
    i2 = operations->resource->size[0] * operations->resource->size[1];
    for (kEnd = 0; kEnd < i2; kEnd++) {
      c_operations->data[kEnd] = operations->resource->data[kEnd];
    }

    count_parents(operations->numbers->data[qEnd], &parent_count->
                  data[operations->numbers->data[qEnd] - 1], b_operations,
                  c_operations);
    if (parent_count->data[operations->numbers->data[qEnd] - 1] < Latency) {
      //  the number of generations of parents is the location in the ASAP Schedule 
      Time_Frames->data[operations->numbers->data[qEnd] - 1] =
        parent_count->data[operations->numbers->data[qEnd] - 1] + 1.0;

      //  First part of Time Frame is Soonest possible time
    } else {
      // disp(['Cant Schedule Node #' num2str(v) ' in ASAP'])
    }

    qEnd++;
  }

  emxFree_uint16_T(&c_operations);
  emxFree_uint16_T(&b_operations);

  // ALAP Schedule
  //  ALAP = cell(Latency,1);
  kEnd = parent_count->size[0];
  parent_count->size[0] = operations->numbers->size[1];
  emxEnsureCapacity((emxArray__common *)parent_count, kEnd, (int)sizeof(double));
  i2 = operations->numbers->size[1];
  for (kEnd = 0; kEnd < i2; kEnd++) {
    parent_count->data[kEnd] = 0.0;
  }

  //  Look for operations that have no children
  qEnd = 0;
  emxInit_uint16_T(&d_operations, 2);
  emxInit_uint16_T(&e_operations, 2);
  while (qEnd <= operations->numbers->size[1] - 1) {
    //  find how many generations of children this node has
    kEnd = d_operations->size[0] * d_operations->size[1];
    d_operations->size[0] = operations->children->size[0];
    d_operations->size[1] = operations->children->size[1];
    emxEnsureCapacity((emxArray__common *)d_operations, kEnd, (int)sizeof
                      (unsigned short));
    i2 = operations->children->size[0] * operations->children->size[1];
    for (kEnd = 0; kEnd < i2; kEnd++) {
      d_operations->data[kEnd] = operations->children->data[kEnd];
    }

    kEnd = e_operations->size[0] * e_operations->size[1];
    e_operations->size[0] = 1;
    e_operations->size[1] = operations->resource->size[1];
    emxEnsureCapacity((emxArray__common *)e_operations, kEnd, (int)sizeof
                      (unsigned short));
    i2 = operations->resource->size[0] * operations->resource->size[1];
    for (kEnd = 0; kEnd < i2; kEnd++) {
      e_operations->data[kEnd] = operations->resource->data[kEnd];
    }

    count_children(operations->numbers->data[qEnd], &parent_count->
                   data[operations->numbers->data[qEnd] - 1], d_operations,
                   e_operations);
    if (parent_count->data[operations->numbers->data[qEnd] - 1] < Latency) {
      //  the number of generations of children is the reverse location in the ALAP Schedule 
      ndbl = rt_roundd_snf((double)Latency - parent_count->data
                           [operations->numbers->data[qEnd] - 1]);
      if (ndbl < 65536.0) {
        if (ndbl >= 0.0) {
          b_x = (unsigned short)ndbl;
        } else {
          b_x = 0;
        }
      } else if (ndbl >= 65536.0) {
        b_x = MAX_uint16_T;
      } else {
        b_x = 0;
      }

      Time_Frames->data[(operations->numbers->data[qEnd] + Time_Frames->size[0])
        - 1] = b_x;

      //  Second part of Time Frame is Latest possible time
    } else {
      // disp(['Cant Schedule Node #' num2str(v) ' in ALAP'])
    }

    qEnd++;
  }

  emxFree_uint16_T(&e_operations);
  emxFree_uint16_T(&d_operations);
  emxInit_real_T(&Operation_Probability, 2);
  emxInit_real_T(&Type_Probability, 2);
  emxInit_real_T(&Self_Force, 2);
  emxInit_real_T(&time_options, 2);
  emxInit_int32_T(&min_force_cycle, 1);
  emxInit_uint16_T(&b, 2);
  emxInit_int32_T1(&idx, 2);
  emxInit_int32_T(&iwork, 1);
  emxInit_real_T(&minval, 2);
  emxInit_boolean_T(&x, 2);
  emxInit_int32_T(&jj, 1);
  while (!all_operations_scheduled) {
    //     %% Compute the operations and type probabilities
    kEnd = Operation_Probability->size[0] * Operation_Probability->size[1];
    Operation_Probability->size[0] = operations->numbers->size[1];
    Operation_Probability->size[1] = Latency;
    emxEnsureCapacity((emxArray__common *)Operation_Probability, kEnd, (int)
                      sizeof(double));
    i2 = operations->numbers->size[1] * Latency;
    for (kEnd = 0; kEnd < i2; kEnd++) {
      Operation_Probability->data[kEnd] = 0.0;
    }

    na = operations->resource->size[1];
    n = operations->resource->size[1] + 1;
    kEnd = idx->size[0] * idx->size[1];
    idx->size[0] = 1;
    idx->size[1] = operations->resource->size[1];
    emxEnsureCapacity((emxArray__common *)idx, kEnd, (int)sizeof(int));
    i2 = operations->resource->size[1];
    for (kEnd = 0; kEnd < i2; kEnd++) {
      idx->data[kEnd] = 0;
    }

    if (operations->resource->size[1] != 0) {
      j = operations->resource->size[1];
      kEnd = iwork->size[0];
      iwork->size[0] = j;
      emxEnsureCapacity((emxArray__common *)iwork, kEnd, (int)sizeof(int));
      for (k = 1; k <= n - 2; k += 2) {
        all_operations_scheduled = (operations->resource->data[k - 1] <=
          operations->resource->data[k]);
        if (all_operations_scheduled) {
          idx->data[k - 1] = k;
          idx->data[k] = k + 1;
        } else {
          idx->data[k - 1] = k + 1;
          idx->data[k] = k;
        }
      }

      if ((operations->resource->size[1] & 1) != 0) {
        idx->data[operations->resource->size[1] - 1] = operations->
          resource->size[1];
      }

      i = 2;
      while (i < n - 1) {
        i2 = i << 1;
        j = 1;
        for (pEnd = 1 + i; pEnd < n; pEnd = qEnd + i) {
          p = j;
          q = pEnd;
          qEnd = j + i2;
          if (qEnd > n) {
            qEnd = n;
          }

          k = 0;
          kEnd = qEnd - j;
          while (k + 1 <= kEnd) {
            all_operations_scheduled = (operations->resource->data[idx->data[p -
              1] - 1] <= operations->resource->data[idx->data[q - 1] - 1]);
            if (all_operations_scheduled) {
              iwork->data[k] = idx->data[p - 1];
              p++;
              if (p == pEnd) {
                while (q < qEnd) {
                  k++;
                  iwork->data[k] = idx->data[q - 1];
                  q++;
                }
              }
            } else {
              iwork->data[k] = idx->data[q - 1];
              q++;
              if (q == qEnd) {
                while (p < pEnd) {
                  k++;
                  iwork->data[k] = idx->data[p - 1];
                  p++;
                }
              }
            }

            k++;
          }

          for (k = 0; k + 1 <= kEnd; k++) {
            idx->data[(j + k) - 1] = iwork->data[k];
          }

          j = qEnd;
        }

        i = i2;
      }
    }

    kEnd = b->size[0] * b->size[1];
    b->size[0] = 1;
    b->size[1] = operations->resource->size[1];
    emxEnsureCapacity((emxArray__common *)b, kEnd, (int)sizeof(unsigned short));
    for (k = 0; k + 1 <= na; k++) {
      b->data[k] = operations->resource->data[idx->data[k] - 1];
    }

    i2 = 0;
    k = 1;
    while (k <= na) {
      b_x = b->data[k - 1];
      do {
        k++;
      } while (!((k > na) || (!(b->data[k - 1] == b_x))));

      i2++;
      b->data[i2 - 1] = b_x;
    }

    if (1 > i2) {
      kEnd = 0;
    } else {
      kEnd = i2;
    }

    j = Type_Probability->size[0] * Type_Probability->size[1];
    Type_Probability->size[0] = kEnd;
    Type_Probability->size[1] = Latency;
    emxEnsureCapacity((emxArray__common *)Type_Probability, j, (int)sizeof
                      (double));
    i2 = kEnd * Latency;
    for (kEnd = 0; kEnd < i2; kEnd++) {
      Type_Probability->data[kEnd] = 0.0;
    }

    for (b_x = 1; b_x <= Latency; b_x++) {
      // Loop through each cycle
      for (qEnd = 0; qEnd < operations->numbers->size[1]; qEnd++) {
        // Loop through each operations
        //  if the current cycle is within the time frame for this operation
        if ((b_x >= Time_Frames->data[qEnd]) && (b_x <= Time_Frames->data[qEnd +
             Time_Frames->size[0]])) {
          //  then the probability is the recipricol of the range of the Time Frame 
          Operation_Probability->data[qEnd + Operation_Probability->size[0] *
            (b_x - 1)] = 1.0 / ((Time_Frames->data[qEnd + Time_Frames->size[0]]
            - Time_Frames->data[qEnd]) + 1.0);
        } else {
          //  else the probablility is 0
          Operation_Probability->data[qEnd + Operation_Probability->size[0] *
            (b_x - 1)] = 0.0;
        }

        Type_Probability->data[(operations->resource->data[qEnd] +
          Type_Probability->size[0] * (b_x - 1)) - 1] +=
          Operation_Probability->data[qEnd + Operation_Probability->size[0] *
          (b_x - 1)];

        //  +=
      }
    }

    //     %% Compute the self-forces, predecessor/succesor forces and total forces 
    //  Calculate All Self Forces
    kEnd = Self_Force->size[0] * Self_Force->size[1];
    Self_Force->size[0] = operations->numbers->size[1];
    Self_Force->size[1] = Latency;
    emxEnsureCapacity((emxArray__common *)Self_Force, kEnd, (int)sizeof(double));
    i2 = operations->numbers->size[1] * Latency;
    for (kEnd = 0; kEnd < i2; kEnd++) {
      Self_Force->data[kEnd] = 0.0;
    }

    for (b_x = 1; b_x <= Latency; b_x++) {
      // Loop through each cycle
      for (qEnd = 0; qEnd < operations->numbers->size[1]; qEnd++) {
        // Loop through each operations
        //  if the current cycle is within the time frame for this operation
        if ((b_x >= Time_Frames->data[qEnd]) && (b_x <= Time_Frames->data[qEnd +
             Time_Frames->size[0]])) {
          for (c = 1; c <= Latency; c++) {
            // Loop through each cycle again
            Self_Force->data[qEnd + Self_Force->size[0] * (b_x - 1)] +=
              Type_Probability->data[(operations->resource->data[qEnd] +
              Type_Probability->size[0] * (c - 1)) - 1] * ((double)(c == b_x) -
              Operation_Probability->data[qEnd + Operation_Probability->size[0] *
              (c - 1)]);

            //  +=
          }
        } else {
          //  set self force to Inf becuase operation can't be scheduled this cycle 
          Self_Force->data[qEnd + Self_Force->size[0] * (b_x - 1)] = rtInf;
        }
      }
    }

    //  Calculate All Predecessor/Successor Forces
    kEnd = Operation_Probability->size[0] * Operation_Probability->size[1];
    Operation_Probability->size[0] = operations->numbers->size[1];
    Operation_Probability->size[1] = Latency;
    emxEnsureCapacity((emxArray__common *)Operation_Probability, kEnd, (int)
                      sizeof(double));
    i2 = operations->numbers->size[1] * Latency;
    for (kEnd = 0; kEnd < i2; kEnd++) {
      Operation_Probability->data[kEnd] = 0.0;
    }

    kEnd = Type_Probability->size[0] * Type_Probability->size[1];
    Type_Probability->size[0] = operations->numbers->size[1];
    Type_Probability->size[1] = Latency;
    emxEnsureCapacity((emxArray__common *)Type_Probability, kEnd, (int)sizeof
                      (double));
    i2 = operations->numbers->size[1] * Latency;
    for (kEnd = 0; kEnd < i2; kEnd++) {
      Type_Probability->data[kEnd] = 0.0;
    }

    for (b_x = 1; b_x <= Latency; b_x++) {
      // Loop through each cycle
      for (qEnd = 0; qEnd < operations->numbers->size[1]; qEnd++) {
        // Loop through each operations
        //  if the current cycle is within the time frame for this operation
        if ((b_x >= Time_Frames->data[qEnd]) && (b_x <= Time_Frames->data[qEnd +
             Time_Frames->size[0]])) {
          kEnd = operations->parents->size[1];
          for (p = 0; p < kEnd; p++) {
            // Loop through all of this operation's parents
            //  will scheduling v at time t effect the schedule for p?
            if ((b_x >= Time_Frames->data[p]) && (b_x <= Time_Frames->data[p +
                 Time_Frames->size[0]])) {
              //  Find all the options for scheduling the parent
              if (Time_Frames->data[p + Time_Frames->size[0]] <
                  Time_Frames->data[p]) {
                j = time_options->size[0] * time_options->size[1];
                time_options->size[0] = 1;
                time_options->size[1] = 0;
                emxEnsureCapacity((emxArray__common *)time_options, j, (int)
                                  sizeof(double));
              } else if ((rtIsInf(Time_Frames->data[p]) || rtIsInf
                          (Time_Frames->data[p + Time_Frames->size[0]])) &&
                         (Time_Frames->data[p] == Time_Frames->data[p +
                          Time_Frames->size[0]])) {
                j = time_options->size[0] * time_options->size[1];
                time_options->size[0] = 1;
                time_options->size[1] = 1;
                emxEnsureCapacity((emxArray__common *)time_options, j, (int)
                                  sizeof(double));
                time_options->data[0] = rtNaN;
              } else if (std::floor(Time_Frames->data[p]) == Time_Frames->data[p])
              {
                ndbl = Time_Frames->data[p];
                apnd = Time_Frames->data[p + Time_Frames->size[0]];
                j = time_options->size[0] * time_options->size[1];
                time_options->size[0] = 1;
                time_options->size[1] = (int)std::floor(apnd - ndbl) + 1;
                emxEnsureCapacity((emxArray__common *)time_options, j, (int)
                                  sizeof(double));
                i2 = (int)std::floor(apnd - ndbl);
                for (j = 0; j <= i2; j++) {
                  time_options->data[time_options->size[0] * j] = ndbl + (double)
                    j;
                }
              } else {
                ndbl = std::floor((Time_Frames->data[p + Time_Frames->size[0]] -
                                   Time_Frames->data[p]) + 0.5);
                apnd = Time_Frames->data[p] + ndbl;
                cdiff = apnd - Time_Frames->data[p + Time_Frames->size[0]];
                absa = std::abs(Time_Frames->data[p]);
                absb = std::abs(Time_Frames->data[p + Time_Frames->size[0]]);
                if ((absa >= absb) || rtIsNaN(absb)) {
                  absb = absa;
                }

                if (std::abs(cdiff) < 4.4408920985006262E-16 * absb) {
                  ndbl++;
                  apnd = Time_Frames->data[p + Time_Frames->size[0]];
                } else if (cdiff > 0.0) {
                  apnd = Time_Frames->data[p] + (ndbl - 1.0);
                } else {
                  ndbl++;
                }

                if (ndbl >= 0.0) {
                  n = (int)ndbl;
                } else {
                  n = 0;
                }

                j = time_options->size[0] * time_options->size[1];
                time_options->size[0] = 1;
                time_options->size[1] = n;
                emxEnsureCapacity((emxArray__common *)time_options, j, (int)
                                  sizeof(double));
                if (n > 0) {
                  time_options->data[0] = Time_Frames->data[p];
                  if (n > 1) {
                    time_options->data[n - 1] = apnd;
                    i2 = (n - 1) / 2;
                    for (k = 1; k < i2; k++) {
                      time_options->data[k] = Time_Frames->data[p] + (double)k;
                      time_options->data[(n - k) - 1] = apnd - (double)k;
                    }

                    if (i2 << 1 == n - 1) {
                      time_options->data[i2] = (Time_Frames->data[p] + apnd) /
                        2.0;
                    } else {
                      time_options->data[i2] = Time_Frames->data[p] + (double)i2;
                      time_options->data[i2 + 1] = apnd - (double)i2;
                    }
                  }
                }
              }

              //  all times when p can be scheduled
              i2 = time_options->size[1] - 1;
              pEnd = 0;
              for (i = 0; i <= i2; i++) {
                if (time_options->data[i] != b_x) {
                  pEnd++;
                }
              }

              j = 0;
              for (i = 0; i <= i2; i++) {
                if (time_options->data[i] != b_x) {
                  time_options->data[j] = time_options->data[i];
                  j++;
                }
              }

              j = time_options->size[0] * time_options->size[1];
              time_options->size[0] = 1;
              time_options->size[1] = pEnd;
              emxEnsureCapacity((emxArray__common *)time_options, j, (int)sizeof
                                (double));

              //  except the current time t
              // Loop through all these options to add up the force
              for (i2 = 0; i2 < time_options->size[1]; i2++) {
                //  add the self force of this parent at this t
                Operation_Probability->data[qEnd + Operation_Probability->size[0]
                  * (b_x - 1)] += Self_Force->data[p + Self_Force->size[0] *
                  ((int)time_options->data[i2] - 1)];

                //  +=
              }
            } else {
              //  Parent uneffected
              //  temporary for bookkeeping
            }
          }

          kEnd = operations->children->size[1];
          for (q = 0; q < kEnd; q++) {
            // Loop through all of this operation's children
            //  will scheduling v at time t effect the schedule for p?
            if ((b_x >= Time_Frames->data[q]) && (b_x <= Time_Frames->data[q +
                 Time_Frames->size[0]])) {
              //  Find all the options for scheduling the child
              if (Time_Frames->data[q + Time_Frames->size[0]] <
                  Time_Frames->data[q]) {
                j = time_options->size[0] * time_options->size[1];
                time_options->size[0] = 1;
                time_options->size[1] = 0;
                emxEnsureCapacity((emxArray__common *)time_options, j, (int)
                                  sizeof(double));
              } else if ((rtIsInf(Time_Frames->data[q]) || rtIsInf
                          (Time_Frames->data[q + Time_Frames->size[0]])) &&
                         (Time_Frames->data[q] == Time_Frames->data[q +
                          Time_Frames->size[0]])) {
                j = time_options->size[0] * time_options->size[1];
                time_options->size[0] = 1;
                time_options->size[1] = 1;
                emxEnsureCapacity((emxArray__common *)time_options, j, (int)
                                  sizeof(double));
                time_options->data[0] = rtNaN;
              } else if (std::floor(Time_Frames->data[q]) == Time_Frames->data[q])
              {
                ndbl = Time_Frames->data[q];
                apnd = Time_Frames->data[q + Time_Frames->size[0]];
                j = time_options->size[0] * time_options->size[1];
                time_options->size[0] = 1;
                time_options->size[1] = (int)std::floor(apnd - ndbl) + 1;
                emxEnsureCapacity((emxArray__common *)time_options, j, (int)
                                  sizeof(double));
                i2 = (int)std::floor(apnd - ndbl);
                for (j = 0; j <= i2; j++) {
                  time_options->data[time_options->size[0] * j] = ndbl + (double)
                    j;
                }
              } else {
                ndbl = std::floor((Time_Frames->data[q + Time_Frames->size[0]] -
                                   Time_Frames->data[q]) + 0.5);
                apnd = Time_Frames->data[q] + ndbl;
                cdiff = apnd - Time_Frames->data[q + Time_Frames->size[0]];
                absa = std::abs(Time_Frames->data[q]);
                absb = std::abs(Time_Frames->data[q + Time_Frames->size[0]]);
                if ((absa >= absb) || rtIsNaN(absb)) {
                  absb = absa;
                }

                if (std::abs(cdiff) < 4.4408920985006262E-16 * absb) {
                  ndbl++;
                  apnd = Time_Frames->data[q + Time_Frames->size[0]];
                } else if (cdiff > 0.0) {
                  apnd = Time_Frames->data[q] + (ndbl - 1.0);
                } else {
                  ndbl++;
                }

                if (ndbl >= 0.0) {
                  n = (int)ndbl;
                } else {
                  n = 0;
                }

                j = time_options->size[0] * time_options->size[1];
                time_options->size[0] = 1;
                time_options->size[1] = n;
                emxEnsureCapacity((emxArray__common *)time_options, j, (int)
                                  sizeof(double));
                if (n > 0) {
                  time_options->data[0] = Time_Frames->data[q];
                  if (n > 1) {
                    time_options->data[n - 1] = apnd;
                    i2 = (n - 1) / 2;
                    for (k = 1; k < i2; k++) {
                      time_options->data[k] = Time_Frames->data[q] + (double)k;
                      time_options->data[(n - k) - 1] = apnd - (double)k;
                    }

                    if (i2 << 1 == n - 1) {
                      time_options->data[i2] = (Time_Frames->data[q] + apnd) /
                        2.0;
                    } else {
                      time_options->data[i2] = Time_Frames->data[q] + (double)i2;
                      time_options->data[i2 + 1] = apnd - (double)i2;
                    }
                  }
                }
              }

              //  all times when p can be scheduled
              i2 = time_options->size[1] - 1;
              pEnd = 0;
              for (i = 0; i <= i2; i++) {
                if (time_options->data[i] != b_x) {
                  pEnd++;
                }
              }

              j = 0;
              for (i = 0; i <= i2; i++) {
                if (time_options->data[i] != b_x) {
                  time_options->data[j] = time_options->data[i];
                  j++;
                }
              }

              j = time_options->size[0] * time_options->size[1];
              time_options->size[0] = 1;
              time_options->size[1] = pEnd;
              emxEnsureCapacity((emxArray__common *)time_options, j, (int)sizeof
                                (double));

              //  except the current time t
              // Loop through all these options to add up the force
              for (i2 = 0; i2 < time_options->size[1]; i2++) {
                //  add the self force of this child at this t
                Type_Probability->data[qEnd + Type_Probability->size[0] * (b_x -
                  1)] += Self_Force->data[q + Self_Force->size[0] * ((int)
                  time_options->data[i2] - 1)];

                //  +=
              }
            } else {
              //  Parent uneffected
              //  temporary for bookkeeping
            }
          }
        } else {
          Operation_Probability->data[qEnd + Operation_Probability->size[0] *
            (b_x - 1)] = 0.0;
          Type_Probability->data[qEnd + Type_Probability->size[0] * (b_x - 1)] =
            0.0;
        }
      }
    }

    kEnd = Self_Force->size[0] * Self_Force->size[1];
    emxEnsureCapacity((emxArray__common *)Self_Force, kEnd, (int)sizeof(double));
    i2 = Self_Force->size[0];
    j = Self_Force->size[1];
    i2 *= j;
    for (kEnd = 0; kEnd < i2; kEnd++) {
      Self_Force->data[kEnd] = (Self_Force->data[kEnd] +
        Operation_Probability->data[kEnd]) + Type_Probability->data[kEnd];
    }

    // I would sometimes get really small floating point umbers for Total
    // Force, so that would screw up the min force calculation
    // Total_Force = round(Total_Force, 5); % this rounds Total_Force to 5 decimal places, may not be nessesary 
    //     %% Schedule the operations with the least force and update its time-frame 
    kEnd = minval->size[0] * minval->size[1];
    minval->size[0] = 1;
    minval->size[1] = Self_Force->size[1];
    emxEnsureCapacity((emxArray__common *)minval, kEnd, (int)sizeof(double));
    n = Self_Force->size[0];
    for (i = 0; i + 1 <= Self_Force->size[1]; i++) {
      j = i * n;
      i2 = i * n + 1;
      pEnd = j + n;
      ndbl = Self_Force->data[j];
      if (n > 1) {
        if (rtIsNaN(Self_Force->data[j])) {
          j = i2;
          exitg4 = false;
          while ((!exitg4) && (j + 1 <= pEnd)) {
            i2 = j + 1;
            if (!rtIsNaN(Self_Force->data[j])) {
              ndbl = Self_Force->data[j];
              exitg4 = true;
            } else {
              j++;
            }
          }
        }

        if (i2 < pEnd) {
          while (i2 + 1 <= pEnd) {
            if (Self_Force->data[i2] < ndbl) {
              ndbl = Self_Force->data[i2];
            }

            i2++;
          }
        }
      }

      minval->data[i] = ndbl;
    }

    i2 = 1;
    n = minval->size[1];
    ndbl = minval->data[0];
    if (minval->size[1] > 1) {
      if (rtIsNaN(minval->data[0])) {
        j = 2;
        exitg3 = false;
        while ((!exitg3) && (j <= n)) {
          i2 = j;
          if (!rtIsNaN(minval->data[j - 1])) {
            ndbl = minval->data[j - 1];
            exitg3 = true;
          } else {
            j++;
          }
        }
      }

      if (i2 < minval->size[1]) {
        while (i2 + 1 <= n) {
          if (minval->data[i2] < ndbl) {
            ndbl = minval->data[i2];
          }

          i2++;
        }
      }
    }

    kEnd = x->size[0] * x->size[1];
    x->size[0] = Self_Force->size[0];
    x->size[1] = Self_Force->size[1];
    emxEnsureCapacity((emxArray__common *)x, kEnd, (int)sizeof(boolean_T));
    i2 = Self_Force->size[0] * Self_Force->size[1];
    for (kEnd = 0; kEnd < i2; kEnd++) {
      x->data[kEnd] = (Self_Force->data[kEnd] == ndbl);
    }

    pEnd = x->size[0] * x->size[1];
    q = 0;
    kEnd = iwork->size[0];
    iwork->size[0] = pEnd;
    emxEnsureCapacity((emxArray__common *)iwork, kEnd, (int)sizeof(int));
    kEnd = jj->size[0];
    jj->size[0] = pEnd;
    emxEnsureCapacity((emxArray__common *)jj, kEnd, (int)sizeof(int));
    if (pEnd == 0) {
      kEnd = iwork->size[0];
      iwork->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)iwork, kEnd, (int)sizeof(int));
      kEnd = jj->size[0];
      jj->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)jj, kEnd, (int)sizeof(int));
    } else {
      i2 = 1;
      j = 1;
      exitg2 = false;
      while ((!exitg2) && (j <= x->size[1])) {
        guard1 = false;
        if (x->data[(i2 + x->size[0] * (j - 1)) - 1]) {
          q++;
          iwork->data[q - 1] = i2;
          jj->data[q - 1] = j;
          if (q >= pEnd) {
            exitg2 = true;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }

        if (guard1) {
          i2++;
          if (i2 > x->size[0]) {
            i2 = 1;
            j++;
          }
        }
      }

      if (pEnd == 1) {
        if (q == 0) {
          kEnd = iwork->size[0];
          iwork->size[0] = 0;
          emxEnsureCapacity((emxArray__common *)iwork, kEnd, (int)sizeof(int));
          kEnd = jj->size[0];
          jj->size[0] = 0;
          emxEnsureCapacity((emxArray__common *)jj, kEnd, (int)sizeof(int));
        }
      } else {
        kEnd = iwork->size[0];
        if (1 > q) {
          iwork->size[0] = 0;
        } else {
          iwork->size[0] = q;
        }

        emxEnsureCapacity((emxArray__common *)iwork, kEnd, (int)sizeof(int));
        kEnd = jj->size[0];
        if (1 > q) {
          jj->size[0] = 0;
        } else {
          jj->size[0] = q;
        }

        emxEnsureCapacity((emxArray__common *)jj, kEnd, (int)sizeof(int));
      }
    }

    kEnd = parent_count->size[0];
    parent_count->size[0] = iwork->size[0];
    emxEnsureCapacity((emxArray__common *)parent_count, kEnd, (int)sizeof(double));
    i2 = iwork->size[0];
    for (kEnd = 0; kEnd < i2; kEnd++) {
      parent_count->data[kEnd] = iwork->data[kEnd];
    }

    kEnd = min_force_cycle->size[0];
    min_force_cycle->size[0] = jj->size[0];
    emxEnsureCapacity((emxArray__common *)min_force_cycle, kEnd, (int)sizeof(int));
    i2 = jj->size[0];
    for (kEnd = 0; kEnd < i2; kEnd++) {
      min_force_cycle->data[kEnd] = jj->data[kEnd];
    }

    //  need to find alternative to the 'find' function that will work with c++
    //  if there are multple min Total Force, just pick the first one found that wasn't already scheduled 
    i2 = 0;
    exitg1 = false;
    while ((!exitg1) && (i2 <= parent_count->size[0] - 1)) {
      if (!operations->scheduled->data[(int)parent_count->data[i2] - 1]) {
        ndbl = parent_count->data[i2];
        kEnd = parent_count->size[0];
        parent_count->size[0] = 1;
        emxEnsureCapacity((emxArray__common *)parent_count, kEnd, (int)sizeof
                          (double));
        parent_count->data[0] = ndbl;
        i2 = min_force_cycle->data[0];
        kEnd = min_force_cycle->size[0];
        min_force_cycle->size[0] = 1;
        emxEnsureCapacity((emxArray__common *)min_force_cycle, kEnd, (int)sizeof
                          (int));
        min_force_cycle->data[0] = i2;
        exitg1 = true;
      } else {
        i2++;
      }
    }

    //  add the minimum Force node to the Output Schedule
    //      OUTPUT_SCHEDULE{min_force_cycle} = [min_force_operation, OUTPUT_SCHEDULE{min_force_cycle}]; 
    if (operations->numbers->data[0] == (int)parent_count->data[0]) {
      i2 = 1;
      j = 1;
    } else {
      i2 = 0;
      j = 0;
    }

    i2 *= j;
    for (kEnd = 0; kEnd < i2; kEnd++) {
      OUTPUT_SCHEDULE->data[0] = min_force_cycle->data[kEnd];
    }

    //  mark it as scheduled
    operations->scheduled->data[(int)parent_count->data[0] - 1] = true;

    //  adjust the Time_Frames to lock that node in place
    Time_Frames->data[(int)parent_count->data[0] - 1] = min_force_cycle->data[0];
    Time_Frames->data[((int)parent_count->data[0] + Time_Frames->size[0]) - 1] =
      min_force_cycle->data[0];

    //  update the depedant Time_Frames (parents/children)
    all_operations_scheduled = all(operations->scheduled);
  }

  emxFree_int32_T(&jj);
  emxFree_boolean_T(&x);
  emxFree_real_T(&minval);
  emxFree_int32_T(&iwork);
  emxFree_int32_T(&idx);
  emxFree_uint16_T(&b);
  emxFree_int32_T(&min_force_cycle);
  emxFree_real_T(&time_options);
  emxFree_real_T(&Self_Force);
  emxFree_real_T(&Type_Probability);
  emxFree_real_T(&Operation_Probability);
  emxFree_real_T(&parent_count);
  emxFree_real_T(&Time_Frames);
}

//
// File trailer for FORCE_DIRECTED_dot_cpp.cpp
//
// [EOF]
//
