#include "vc.h"

#include "box.h"
#include "kernel.h"
#include "three_vector.h"

const VcBool kVc::kTrue = VcBool(true);
const VcBool kVc::kFalse = VcBool(false);

void Box::Inside(SOA<Precision> const &points, bool *const output) const {
  for (int i = 0; i < points.size(); i += VcFloat::Size) {
    const ThreeVector<VcFloat> point(
      VcFloat(&points.x(i)),
      VcFloat(&points.y(i)),
      VcFloat(&points.z(i))
    );
    VcBool result;
    InsideBox<kVc>(dimensions_, point, &result);
    for (int j = 0; j < VcFloat::Size; ++j) output[i+j] = result[j];
  }
}

void Box::Distance(SOA<Precision> const &points,
                   SOA<Precision> const &directions,
                   Precision *const output) const {
  assert(points.size() == directions.size());
  for (int i = 0; i < points.size(); i += VcFloat::Size) {
    const ThreeVector<VcFloat> point(
      VcFloat(&points.x(i)),
      VcFloat(&points.y(i)),
      VcFloat(&points.z(i))
    );
    const ThreeVector<VcFloat> direction(
      VcFloat(&directions.x(i)),
      VcFloat(&directions.y(i)),
      VcFloat(&directions.z(i))
    );
    VcFloat result;
    DistanceToBox<kVc>(dimensions_, point, direction, &result);
    result.store(&output[i]);
  }
}