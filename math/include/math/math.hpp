#pragma once

namespace math {

double const tau        = 6.2831853071795864;
double const tau_over_2 = 3.1415926535897932;
double const tau_over_4 = 1.5707963267948966;

template <typename T>
T lerp(T const& lhs, T const& rhs, float t)
{
    return lhs + t * (rhs - lhs);
}

} // namespace math
