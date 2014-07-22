#ifndef BACKENDS_COMMON_H_
#define BACKENDS_COMMON_H_

#include <cmath>

#if (defined(__CUDACC__) || defined(__NVCC__))
#define BACKENDS_NVCC
#define BACKENDS_CUDA_HEADER_DEVICE __device__
#define BACKENDS_CUDA_HEADER_BOTH __host__ __device__
#else // Not compiling with NVCC
#define BACKENDS_STD_CXX11
#define BACKENDS_CUDA_HEADER_DEVICE
#define BACKENDS_CUDA_HEADER_BOTH
#endif

#undef NULL
#ifdef BACKENDS_STD_CXX11
#define NULL nullptr
#define BACKENDS_CONSTEXPR constexpr
#else
#define NULL 0
#define BACKENDS_CONSTEXPR const
#endif

#ifdef BACKENDS_FLOAT_PRECISION
typedef float Precision;
#else
typedef double Precision;
#endif

BACKENDS_CONSTEXPR Precision kInfinity = INFINITY;
BACKENDS_CONSTEXPR Precision kTiny = 1e-20;
BACKENDS_CONSTEXPR int kAlignment = 32;

template <typename Type> class ThreeVector;

class Box;

template <typename Type> class SOA;

#endif // BACKENDS_COMMON_H_