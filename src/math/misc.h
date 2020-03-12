
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_MISC_H
#define CPMML_MISC_H

#include <cmath>
#include <functional>
#include <vector>

#include "boost/integral.h"
#include "utils/utils.h"

/**
 * @defgroup GenericMathFunctions
 *
 */
//@{
inline double closest0or1(const double &value) {
  if (value < 0) return 0;
  if (value > 1) return 1;

  return value;
}

inline double probit(const double a) {
  std::function<double(double)> function = [](double a) {
    return (1 / std::sqrt(2 * M_PI)) * std::exp(-0.5 * std::pow(a, 2));
  };
  return ::integral(::double_min(), a, 0.01, function);
}

inline double logit(const double a) { return 1 / (1 + std::exp(-a)); }

inline double _exp(const double a) { return std::exp(a); }

inline double cloglog(const double a) { return 1 - std::exp(-std::exp(a)); }

inline double loglog(const double a) { return std::exp(-std::exp(a)); }

inline double cauchit(const double a) { return 0.5 + (1 / M_PI) * atan(a); }

inline double _round(const double a) { return std::round(a); }

inline double _floor(const double a) { return std::floor(a); }

inline double _ceil(const double a) { return std::ceil(a); }

inline double _identity(const double a) { return a; }
//@}
#endif
