#ifndef BACKENDS_THREEVECTOR_H_
#define BACKENDS_THREEVECTOR_H_

#include "common.h"

#include <ostream>

template <typename Type>
class ThreeVector {

private:

  Type vec_[3];
  typedef ThreeVector<Type> VecType;

public:

  ThreeVector() {}

  ThreeVector(Type const &scalar) : vec_{scalar, scalar, scalar} {}

  ThreeVector(Type const &x, Type const &y, Type const &z)
      : vec_{x, y, z} {}

  ThreeVector(ThreeVector const &other) {
    *this = other;
  }

  ~ThreeVector() {}

  inline Type& operator[](const int index) { return vec_[index]; }
  inline Type operator[](const int index) const { return vec_[index]; }

  ThreeVector& operator=(VecType const &other) {
    vec_[0] = other[0];
    vec_[1] = other[1];
    vec_[2] = other[2];
    return *this;
  }

  inline Type Length() const;

  inline void Normalize();

  // Inplace binary operators

  #define INPLACE_BINARY_OP(OPERATOR) \
  BACKENDS_CUDA_HEADER_BOTH \
  inline VecType& operator OPERATOR(VecType const &other) { \
    vec_[0] OPERATOR other.vec_[0]; \
    vec_[1] OPERATOR other.vec_[1]; \
    vec_[2] OPERATOR other.vec_[2]; \
    return *this; \
  } \
  BACKENDS_CUDA_HEADER_BOTH \
  inline VecType& operator OPERATOR(Type const &scalar) { \
    vec_[0] OPERATOR scalar; \
    vec_[1] OPERATOR scalar; \
    vec_[2] OPERATOR scalar; \
    return *this; \
  }
  INPLACE_BINARY_OP(+=)
  INPLACE_BINARY_OP(-=)
  INPLACE_BINARY_OP(*=)
  INPLACE_BINARY_OP(/=)
  #undef INPLACE_BINARY_OP

  // Binary operators

  #define BINARY_OP(OPERATOR, INPLACE) \
  BACKENDS_CUDA_HEADER_BOTH \
  inline VecType operator OPERATOR(VecType const &other) const { \
    VecType result(*this); \
    result INPLACE other; \
    return result; \
  } \
  BACKENDS_CUDA_HEADER_BOTH \
  inline VecType operator OPERATOR(Type const &scalar) const { \
    VecType result(*this); \
    result INPLACE scalar; \
    return result; \
  }
  BINARY_OP(+, +=)
  BINARY_OP(-, -=)
  BINARY_OP(*, *=)
  BINARY_OP(/, /=)
  #undef BINARY_OP

};

template <typename Type>
inline std::ostream& operator<<(std::ostream &os,
                                ThreeVector<Type> const &vec) {
  os << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
  return os;
}

template <typename Type>
Type ThreeVector<Type>::Length() const {
  return std::sqrt(vec_[0]*vec_[0] + vec_[1]*vec_[1] + vec_[2]*vec_[2]);
}

template <typename Type>
void ThreeVector<Type>::Normalize() {
  const Type inv_length = 1. / Length();
  *this *= inv_length;
}

#define THREEVECTOR_BINARY_OP(OPERATOR, INPLACE) \
template <typename Type> \
BACKENDS_CUDA_HEADER_BOTH \
ThreeVector<Type> operator OPERATOR(const ThreeVector<Type> &lhs, \
                                    const ThreeVector<Type> &rhs) { \
  ThreeVector<Type> result(lhs); \
  result INPLACE rhs; \
  return result; \
} \
template <typename Type, typename ScalarType> \
BACKENDS_CUDA_HEADER_BOTH \
ThreeVector<Type> operator OPERATOR(ThreeVector<Type> const &lhs, \
                                    const ScalarType rhs) { \
  ThreeVector<Type> result(lhs); \
  result INPLACE rhs; \
  return result; \
} \
template <typename Type, typename ScalarType> \
BACKENDS_CUDA_HEADER_BOTH \
ThreeVector<Type> operator OPERATOR(const ScalarType rhs, \
                                    ThreeVector<Type> const &lhs) { \
  ThreeVector<Type> result(rhs); \
  result INPLACE lhs; \
  return result; \
}
THREEVECTOR_BINARY_OP(+, +=)
THREEVECTOR_BINARY_OP(-, -=)
THREEVECTOR_BINARY_OP(*, *=)
THREEVECTOR_BINARY_OP(/, /=)
#undef THREEVECTOR_BINARY_OP

#endif // BACKENDS_THREEVECTOR_H_