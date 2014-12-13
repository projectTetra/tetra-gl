#pragma once
#ifndef TETRA_UTILITY_STOPWATCH_HPP
#define TETRA_UTILITY_STOPWATCH_HPP

#include <chrono>

namespace tetra
{
namespace util
{

template <class Duration = std::chrono::nanoseconds>
class Stopwatch
{
public:
  Stopwatch()
  {

    auto num = static_cast<double>( Duration::period::num );
    auto den = static_cast<double>( Duration::period::den );
    multiplier = num / den;
  }

  void tic() { m_start = std::chrono::high_resolution_clock::now(); }

  double toc()
  {
    using namespace std::chrono;
    auto now = high_resolution_clock::now();
    auto dur = duration_cast<Duration>( now - m_start );

    return static_cast<double>( dur.count() ) * multiplier;
  }

private:
  std::chrono::high_resolution_clock::time_point m_start;
  double multiplier{0.0};
};

} /* namespace util */
} /* namespace tetra */

#endif
