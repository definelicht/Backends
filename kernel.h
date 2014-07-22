#ifndef BACKENDS_KERNEL_H_
#define BACKENDS_KERNEL_H_

#include "common.h"
#include "backend.h"
#include "three_vector.h"

#include <iostream>

template <typename Backend>
void InsideBox(ThreeVector<Precision> const &box,
               ThreeVector<typename Backend::Float> const &point,
               typename Backend::Bool *const output) {
  
  ThreeVector<typename Backend::Bool> inside;
  for (int i = 0; i < 3; ++i) inside[i] = Abs(point[i]) < box[i];

  *output = inside[0] && inside[1] && inside[2];
}

template <typename Backend>
void DistanceToBox(ThreeVector<Precision> const &box,
                   ThreeVector<typename Backend::Float> const &point,
                   ThreeVector<typename Backend::Float> const &direction,
                   typename Backend::Float *const output) {

  typedef typename Backend::Float Float;
  typedef typename Backend::Bool Bool;

  Bool done = Backend::kFalse;
  Bool hit;
  Float next, coord1, coord2;
  ThreeVector<Float> safety(
    Abs(point[0]) - box[0],
    Abs(point[1]) - box[1],
    Abs(point[2]) - box[2]
  );

  // x
  next = safety[0] / Abs(direction[0] + kTiny);
  coord1 = point[1] + next * direction[1];
  coord2 = point[2] + next * direction[2];
  hit = safety[0] > 0 &&
        point[0] * direction[0] < 0 &&
        Abs(coord1) <= box[1] &&
        Abs(coord2) <= box[2];
  MaskedAssign(!done && hit, next, output);
  done |= hit;
  if (done == Backend::kTrue) return;

  // y
  next = safety[1] / Abs(direction[1] + kTiny);
  coord1 = point[0] + next * direction[0];
  coord2 = point[2] + next * direction[2];
  hit = safety[1] > 0 &&
        point[1] * direction[1] < 0 &&
        Abs(coord1) <= box[0] &&
        Abs(coord2) <= box[2];
  MaskedAssign(!done && hit, next, output);
  done |= hit;
  if (done == Backend::kTrue) return;

  // z
  next = safety[2] / Abs(direction[2] + kTiny);
  coord1 = point[0] + next * direction[0];
  coord2 = point[1] + next * direction[1];
  hit = safety[2] > 0 &&
        point[2] * direction[2] < 0 &&
        Abs(coord1) <= box[0] &&
        Abs(coord2) <= box[1];
  MaskedAssign(!done && hit, next, output);
}

#endif // BACKENDS_KERNEL_H_