#include <iostream>

#include "box.h"
#include "eigen.h"
#include "three_vector.h"

int main() {
  double arr[] = {1, 2, 3, 4};
  EigenFloat a(&arr[0]);
  EigenFloat b(&arr[0]);
  EigenFloat c(&arr[0]);
  ThreeVector<EigenFloat> vec(a, b, c);
  std::cout << vec << "\n";
  return 0;
}