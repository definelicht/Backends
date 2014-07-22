#ifndef BACKENDS_VC_H_
#define BACKENDS_VC_H_

#include <Vc/Vc>
#include "common.h"

struct kVc {
  typedef Vc::Vector<Precision> Float;
  typedef Vc::Vector<int>       Int;
  typedef Float::Mask           Bool;
  const static Bool kTrue;
  const static Bool kFalse;
};

typedef kVc::Float VcFloat;
typedef kVc::Int   VcInt;
typedef kVc::Bool  VcBool;

VcFloat Abs(VcFloat const &input) {
  return Vc::abs(input);
}

template <typename Type>
void MaskedAssign(VcBool const &condition,
                  Type const &then_value,
                  Type *const output) {
  (*output)(condition) = then_value;
}

#endif // BACKENDS_VC_H_