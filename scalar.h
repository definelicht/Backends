#ifndef BACKENDS_VC_H_
#define BACKENDS_VC_H_

#include "common.h"

struct kScalar {
  typedef Precision Float;
  typedef int       Int;
  typedef bool      Bool;
  static const Bool kTrue = true;
  static const Bool kFalse = false;
};

typedef kScalar::Float ScalarFloat;
typedef kScalar::Int   ScalarInt;
typedef kScalar::Bool  ScalarBool;

inline ScalarFloat Abs(const ScalarFloat input) {
  return std::fabs(input);
}

template <typename Type>
inline void MaskedAssign(const ScalarBool condition,
                  Type const &then_value,
                  Type *const output) {
  *output = (condition) ? then_value : *output;
}

#endif // BACKENDS_VC_H_