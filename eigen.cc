#include "eigen.h"

#include "box.h"
#include "kernel.h"
#include "three_vector.h"

#if VECGEOM_VECTOR_BIT_SIZE == 32
const EigenBool kEigen::kTrue = EigenBool(true, true, true, true);
const EigenBool kEigen::kFalse = EigenBool(false, false, false, false);
#elif VECGEOM_VECTOR_BIT_SIZE == 16
const EigenBool kEigen::kTrue = EigenBool(true, true);
const EigenBool kEigen::kFalse = EigenBool(false, false);
#else
#error Only vector sizes of 32 and 16 bits are supported.
#endif

void Box::Inside(SOA<Precision> const &points, bool *const output) const {
  for (int i = 0; i < points.size(); i += kVectorSize) {
    const ThreeVector<EigenFloat> point(
      EigenFloat(&points.x(i)),
      EigenFloat(&points.y(i)),
      EigenFloat(&points.z(i))
    );
    EigenBool result;
    InsideBox<kEigen>(dimensions_, point, &result);
    for (int j = 0; j < kVectorSize; ++j) output[i+j] = result[j];
  }
}

void Box::Distance(SOA<Precision> const &points,
                   SOA<Precision> const &directions,
                   Precision *const output) const {
  assert(points.size() == directions.size());
  for (int i = 0; i < points.size(); i += kVectorSize) {
    const ThreeVector<EigenFloat> point(
      EigenFloat(&points.x(i)),
      EigenFloat(&points.y(i)),
      EigenFloat(&points.z(i))
    );
    const ThreeVector<EigenFloat> direction(
      EigenFloat(&directions.x(i)),
      EigenFloat(&directions.y(i)),
      EigenFloat(&directions.z(i))
    );
    EigenFloat result;
    DistanceToBox<kEigen>(dimensions_, point, direction, &result);
    for (int j = 0; j < kVectorSize; ++j) output[i+j] = result[j];
  }
}