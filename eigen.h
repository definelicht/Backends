#ifndef BACKENDS_EIGEN_H_
#define BACKENDS_EIGEN_H_

#include <Eigen/Core>
#include "common.h"

#if defined(__MIC__)
#define VECGEOM_VECTOR_BIT_SIZE 64
#elif defined(__AVX__)
#define VECGEOM_VECTOR_BIT_SIZE 32
#else // Assume SSE
#define VECGEOM_VECTOR_BIT_SIZE 16
#endif

constexpr int kVectorSize = VECGEOM_VECTOR_BIT_SIZE / sizeof(Precision);

struct kEigen {
  typedef Eigen::Array<Precision, kVectorSize, 1> Float;
  typedef Eigen::Array<int, kVectorSize, 1>       Int;
  typedef Eigen::Array<bool, kVectorSize, 1>      Bool;
  static const Bool kTrue;
  static const Bool kFalse;
};

typedef kEigen::Float EigenFloat;
typedef kEigen::Int   EigenInt;
typedef kEigen::Bool  EigenBool;

inline EigenFloat Abs(EigenFloat const &input) {
  return input.abs();
}

template <typename Type>
void MaskedAssign(EigenBool const &condition,
                  Type const &then_value,
                  Type *const output) {
  for (int i = 0; i < kVectorSize; ++i) {
    output->coeffRef(i) = condition.coeff(i) ? then_value.coeff(i)
                                             : output->coeff(i);
  }
}

inline EigenBool operator!(EigenBool const &input) {
  EigenBool output;
  for (int i = 0; i < kVectorSize; ++i) output.coeffRef(i) = !input.coeff(i);
  return output;
}

inline EigenBool& operator|=(EigenBool &lhs, EigenBool const &rhs) {
  for (int i = 0; i < kVectorSize; ++i) lhs.coeffRef(i) |= rhs.coeff(i);
  return lhs;
}

inline bool operator==(EigenBool const &lhs, EigenBool const &rhs) {
  for (int i = 0; i < kVectorSize; ++i) {
    if (lhs.coeff(i) != rhs.coeff(i)) return false;
  }
  return true;
}

#endif // BACKENDS_EIGEN_H_