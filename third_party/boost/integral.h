#ifndef CPMML_THIRD_PARTY_BOOST_INTEGRAL_H_
#define CPMML_THIRD_PARTY_BOOST_INTEGRAL_H_

/**
 * Copyright © 2002-2013 John Maddock and Christopher Kormanyos
 */
template <class F>
inline double integral(const double a, const double b, const double tol, F function) {
  unsigned n = 1U;

  double h = (b - a);
  double I = (function(a) + function(b)) * (h / 2);

  for (unsigned k = 0U; k < 8U; k++) {
    h /= 2;

    double sum(0);
    for (unsigned j = 1U; j <= n; j++) {
      sum += function(a + (double((j * 2) - 1) * h));
    }

    const double I0 = I;
    I = (I / 2) + (h * sum);

    const double ratio = I0 / I;
    const double delta = ratio - 1;
    const double delta_abs = ((delta < 0) ? -delta : delta);

    if ((k > 1U) && (delta_abs < tol)) {
      break;
    }

    n *= 2U;
  }

  return I;
}

#endif //CPMML_THIRD_PARTY_BOOST_INTEGRAL_H_
