#include <iostream>
#include <vector>
#include <algorithm>
#include "EVOP.h"

int main() {
  std::vector<double> x_0 {0.0, 0.0};
  // Selección de delta grande porque el mínimo está en (2,4)
  std::vector<double> delta {5.0, 5.0};
  double eps = 1e-4;

  auto results = EVOP(x_0, delta, eps);

  for(auto &result: results){
    std:: cout << "x = (" ;
    for(size_t i = 0; i < result.first.size(); ++i){
      std::cout << result.first[i] << ",)"[i == (result.first.size() - 1)];
    }

    std::cout << ", f(x) = " << function(result.first) << '\n';
  }

  std::cout << "number of iterations: " << results.size() << '\n';

  return 0;
}
