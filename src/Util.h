
/** 
 * @brief Misc utilities
 */

#ifndef UTIL_H
#define UTIL_H

#include <cmath>

namespace StopPow
{

template <class T> inline bool approx(T a, T b, T tol)
{
    return (2 * std::abs(a - b) / (a + b) < tol);
}

} // namespace StopPow

#endif