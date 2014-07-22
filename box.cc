#include "box.h"

#include "kernel.h"
#include "scalar.h"

bool Box::Inside(ThreeVector<Precision> const &point) const {
  bool inside = false;
  InsideBox<kScalar>(dimensions_, point, &inside);
  return inside;
}

Precision Box::Distance(ThreeVector<Precision> const &point,
                        ThreeVector<Precision> const &direction) const {
  Precision distance = kInfinity;
  DistanceToBox<kScalar>(dimensions_, point, direction, &distance);
  return distance;
}