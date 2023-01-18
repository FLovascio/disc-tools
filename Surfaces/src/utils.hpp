#pragma once

#include <cmath>
#include <bit>

namespace utils{
auto next_pow2(const unsigned int n)->int{
  return std::bit_ceil(n);
}
};