#include "scalar.h"

#include "box.h"
#include "kernel.h"
#include "three_vector.h"

void Box::Inside(SOA<Precision> const &points, bool *const output) const {
  for (int i = 0; i < points.size(); ++i) {
    InsideBox<kScalar>(dimensions_, points[i], &output[i]);
  }
}

void Box::Distance(SOA<Precision> const &points,
                   SOA<Precision> const &directions,
                   Precision *const output) const {
  assert(points.size() == directions.size());
  for (int i = 0; i < points.size(); ++i) {
    DistanceToBox<kScalar>(dimensions_, points[i], directions[i], &output[i]);
  }
}