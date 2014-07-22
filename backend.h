#ifndef BACKENDS_BACKEND_H_
#define BACKENDS_BACKEND_H_

#ifdef BACKENDS_VC
#include "vc.h"
#elif defined(BACKENDS_CUDA)
#include "cuda.h"
#else
#include "scalar.h"
#endif

#endif // BACKENDS_BACKEND_H_