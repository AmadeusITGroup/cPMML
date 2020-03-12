
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_NORMALIZATIONMETHODS_H
#define CPMML_NORMALIZATIONMETHODS_H

#include <cmath>
#include <functional>
#include <vector>

#include "misc.h"

/**
 * @defgroup NormalizationMethods
 *
 * Implementation of the various normalization methods used in PMML. See <a
 * href="http://dmg.org/pmml/v4-4/Regression.html#xsdType_REGRESSIONNORMALIZATIONMETHOD">PMML
 * Normalization Methods</a>.
 */
//@{
inline std::vector<double> categorical_softmax(const std::vector<double> &values) {
  std::vector<double> exps;
  std::vector<double> result;
  double sum = 0;
  for (auto i = 0u; i < values.size(); i++) {
    exps.push_back(std::exp(values[i]));
    sum += exps.back();
  }

  for (auto i = 0u; i < values.size(); i++) result.push_back(exps[i] / sum);

  return result;
}

inline std::vector<double> categorical_simplemax(const std::vector<double> &values) {
  std::vector<double> result;
  double sum = 0;

  for (const auto &value : values) sum += value;

  for (const auto &value : values) result.push_back(value / sum);

  return result;
}

inline std::vector<double> categorical_none(const std::vector<double> &values) {
  std::vector<double> result;
  double sum = 0;
  size_t i = 0;

  for (i = 0; i < values.size() - 1; i++) {
    sum += values[i];
    result.push_back(values[i]);
  }

  result.push_back(1 - sum);

  if (values.size() == 2) {
    result[0] = closest0or1(result[0]);
    result[1] = closest0or1(result[1]);
  }

  return result;
}

inline std::vector<double> categorical_base(const std::vector<double> &values, std::function<double(double)> function,
                                            const std::string &function_name) {
  if (values.size() != 2) throw cpmml::MathException(function_name + " must have exactly 2 inputs");

  std::vector<double> result;
  result.push_back(function(values[0]));
  result.push_back(1 - result[0]);

  return result;
}

inline std::vector<double> categorical_logit(const std::vector<double> &values) {
  return categorical_base(values, logit, "logit");
}

inline std::vector<double> categorical_probit(const std::vector<double> &values) {
  return categorical_base(values, probit, "probit");
}

inline std::vector<double> categorical_cloglog(const std::vector<double> &values) {
  return categorical_base(values, cloglog, "cloglog");
}

inline std::vector<double> categorical_loglog(const std::vector<double> &values) {
  return categorical_base(values, loglog, "loglog");
}

inline std::vector<double> categorical_cauchit(const std::vector<double> &values) {
  if (values.size() != 2) throw cpmml::MathException("Cauchit must have exactly 2 inputs");

  std::vector<double> result;
  result.push_back(cauchit(values[0]));
  result.push_back(1 - result[0]);

  return result;
}

inline std::vector<double> ordinal_base(const std::vector<double> &values, std::function<double(double)> function) {
  std::vector<double> result;

  result.push_back(function(values[0]));
  for (auto i = 1u; i < values.size(); i++) result.push_back(function(values[i]) - result[i - 1]);
  result.push_back(1 - function(values[values.size() - 2]));

  return result;
}

inline std::vector<double> ordinal_logit(const std::vector<double> &values) { return ordinal_base(values, logit); }

inline std::vector<double> ordinal_probit(const std::vector<double> &values) { return ordinal_base(values, probit); }

inline std::vector<double> ordinal_exp(const std::vector<double> &values) { return ordinal_base(values, _exp); }

inline std::vector<double> ordinal_cloglog(const std::vector<double> &values) { return ordinal_base(values, cloglog); }

inline std::vector<double> ordinal_loglog(const std::vector<double> &values) { return ordinal_base(values, loglog); }

inline std::vector<double> ordinal_cauchit(const std::vector<double> &values) { return ordinal_base(values, cauchit); }

inline std::vector<double> ordinal_none(const std::vector<double> &values) {
  return ordinal_base(values, [](double a) { return a; });
}

inline double single_logit(const double &a) { return logit(a); }

inline double single_softmax(const double &a) { return logit(a); }

inline double single_exp(const double &a) { return _exp(a); }

inline double single_probit(const double &a) { return probit(a); }

inline double single_cloglog(const double &a) { return cloglog(a); }

inline double single_loglog(const double &a) { return loglog(a); }

inline double single_cauchit(const double &a) { return cauchit(a); }

inline double single_none(const double &a) { return a; }
//@}
#endif
