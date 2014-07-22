#ifndef BACKENDS_SOA_H_
#define BACKENDS_SOA_H_

#include <cassert>
#ifndef BACKENDS_NVCC
#include "mm_malloc.h"
#endif

#include "three_vector.h"

template <typename Type>
class SOA {

private:

  int size_, memory_size_;
  Type *x_, *y_, *z_;
  bool allocated_;

public:

  SOA(const int memory_size);

  BACKENDS_CUDA_HEADER_BOTH
  SOA(Type *const x, Type *const y, Type *const z, const int memory_size_);

  SOA(SOA<Type> const &other);

  SOA(SOA<Type> const &other, const int index, const int size);

  ~SOA();

  BACKENDS_CUDA_HEADER_BOTH
  inline ThreeVector<Type> operator[](const int index) const;

  BACKENDS_CUDA_HEADER_BOTH
  inline Type const& x(const int index) const { return x_[index]; }

  BACKENDS_CUDA_HEADER_BOTH
  inline Type const& y(const int index) const { return y_[index]; }

  BACKENDS_CUDA_HEADER_BOTH
  inline Type const& z(const int index) const { return z_[index]; }

  BACKENDS_CUDA_HEADER_BOTH
  inline int size() const { return size_; }

  BACKENDS_CUDA_HEADER_BOTH
  inline int memory_size() const { return memory_size_; }

  BACKENDS_CUDA_HEADER_BOTH
  inline void set(const int index, const Type x, const Type y, const Type z);

  BACKENDS_CUDA_HEADER_BOTH
  inline void set(const int index, ThreeVector<double> const &vec);

};

#ifndef BACKENDS_NVCC

template <typename Type>
SOA<Type>::SOA(const int memory_size)
    : size_(0), memory_size_(memory_size), allocated_(true) {
  x_ = static_cast<Type*>(_mm_malloc(memory_size*sizeof(Type), kAlignment));
  y_ = static_cast<Type*>(_mm_malloc(memory_size*sizeof(Type), kAlignment));
  z_ = static_cast<Type*>(_mm_malloc(memory_size*sizeof(Type), kAlignment));
}

template <typename Type>
SOA<Type>::SOA(SOA<Type> const &other) : SOA<Type>(other.memory_size_) {
  size_ = other.size_;
  for (int i = 0; i < size_; ++i) {
    x_[i] = other.x_[i];
    y_[i] = other.y_[i];
    z_[i] = other.z_[i];
  }
}

template <typename Type>
SOA<Type>::~SOA() {
  if (allocated_) {
    _mm_free(x_);
    _mm_free(y_);
    _mm_free(z_);
  }
}

#endif

template <typename Type>
BACKENDS_CUDA_HEADER_BOTH
SOA<Type>::SOA(Type *const x, Type *const y, Type *const z,
               const int memory_size)
    : size_(0), memory_size_(memory_size), x_(x), y_(y), z_(z),
      allocated_(false) {}

template <typename Type>
SOA<Type>::SOA(SOA<Type> const &other, const int index, const int size)
    : size_(size), memory_size_(size), x_(other.x_[index]), y_(other.y_[index]),
      z_(other.z_[index]), allocated_(false) {
  assert(index + size < other.memory_size_);
}

template <typename Type>
BACKENDS_CUDA_HEADER_BOTH
inline ThreeVector<Type> SOA<Type>::operator[](const int index) const {
  return ThreeVector<Type>(x_[index], y_[index], z_[index]);
}

template <typename Type>
BACKENDS_CUDA_HEADER_BOTH
inline void SOA<Type>::set(const int index, const Type x, const Type y,
                           const Type z) {
  assert(index < memory_size_);
  if (index >= size_) size_ = index + 1;
  x_[index] = x;
  y_[index] = y;
  z_[index] = z;
}

template <typename Type>
BACKENDS_CUDA_HEADER_BOTH
inline void SOA<Type>::set(const int index, ThreeVector<double> const &vec) {
  assert(index < memory_size_);
  if (index >= size_) size_ = index + 1;
  x_[index] = vec[0];
  y_[index] = vec[1];
  z_[index] = vec[2];
}

#endif // BACKENDS_SOA_H_