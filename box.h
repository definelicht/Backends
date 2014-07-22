#ifndef BACKENDS_BOX_H_
#define BACKENDS_BOX_H_

#include "common.h"
#include "three_vector.h"
#include "soa.h"

class Box {

private:

  ThreeVector<Precision> dimensions_;

public:

  Box(ThreeVector<Precision> const &dimensions) : dimensions_(dimensions) {}

  Box(const Precision x, const Precision y, const Precision z)
      : dimensions_(x, y, z) {}

  ~Box() {}

  Precision x() const { return dimensions_[0]; }
  Precision y() const { return dimensions_[1]; }
  Precision z() const { return dimensions_[2]; }
  ThreeVector<Precision> const& dimensions() { return dimensions_; }

  void set_x(const Precision x) { dimensions_[0] = x; }
  void set_y(const Precision y) { dimensions_[1] = y; }
  void set_z(const Precision z) { dimensions_[2] = z; }
  void set_dimensions(ThreeVector<Precision> const &dimensions) {
    dimensions_ = dimensions;
  }

  bool Inside(ThreeVector<Precision> const &point) const;

  void Inside(SOA<Precision> const &points, bool *const output) const;

  Precision Distance(ThreeVector<Precision> const &point,
                     ThreeVector<Precision> const &direction) const;

  void Distance(SOA<Precision> const &points, SOA<Precision> const &directions,
                Precision *const output) const;

};

#endif // BACKENDS_BOX_H_