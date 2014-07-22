#include <iostream>
#include <random>
#include "box.h"
#include "soa.h"
#include "stopwatch.h"

std::mt19937 rng = std::mt19937(0);
std::uniform_real_distribution<> uniform_dist =
    std::uniform_real_distribution<>(0, 1);

double SampleUniform(const Precision min = 0, const Precision max = 1) {
  return min + (max - min) * uniform_dist(rng);
}

ThreeVector<double> SampleUniform(ThreeVector<double> const &dimensions) {
  ThreeVector<double> output = ThreeVector<double>(
    SampleUniform(-dimensions[0], dimensions[0]),
    SampleUniform(-dimensions[1], dimensions[1]),
    SampleUniform(-dimensions[2], dimensions[2])
  );
  return output;
}

ThreeVector<double> SampleDirection() {
  ThreeVector<double> dir(
      (1. - 2. * SampleUniform()),
      (1. - 2. * SampleUniform()),
      (1. - 2. * SampleUniform())
  );
  const Precision inverse_norm =
      1. / std::sqrt(dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2]);
  dir *= inverse_norm;
  return dir;
}

int main() {
  Box box = Box(5, 7, 2);
  constexpr int n_points = 1<<12;
  constexpr int reps = 1<<12;
  constexpr double bias = 0.8;
  ThreeVector<double> points_array[n_points];
  ThreeVector<double> directions_array[n_points];
  SOA<double> points(n_points);
  SOA<double> directions(n_points);
  ThreeVector<double> sample_dimensions = 2.*box.dimensions();
  int hitting = 0;
  for (int p = 0; p < n_points; ++p) {
    points_array[p] = SampleUniform(sample_dimensions);
    points.set(p, points_array[p]);
    bool inside = box.Inside(points_array[p]);
    bool hit = static_cast<double>(hitting) /
               static_cast<double>(n_points) >= bias;
    do {
      directions_array[p] = SampleDirection();
      directions.set(p, directions_array[p]);
      hit = box.Distance(points_array[p], directions_array[p]) < kInfinity;
    } while (!inside && !hit);
  }
  Stopwatch timer;
  bool *inside = new bool[n_points];
  double *distance = new double[n_points];
  timer.Start();
  for (int r = 0; r < reps; ++r) {
    for (int p = 0; p < n_points; ++p) {
      inside[p] = box.Inside(points_array[p]);
      distance[p] = box.Distance(points_array[p], directions_array[p]);
    }
  }
  std::cout << timer.Stop() << "s elapsed for scalar interface.\n";
  timer.Start();
  for (int r = 0; r < reps; ++r) {
    box.Inside(points, inside);
    box.Distance(points, directions, distance);
  }
  std::cout << timer.Stop() << "s elapsed for vector interface.\n";
  return 0;
}