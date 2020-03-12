
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_NORMALIZATIONMETHODTYPE_H
#define CPMML_NORMALIZATIONMETHODTYPE_H

#include <string>
#include "utils/utils.h"

/**
 * @class NormalizationMethodType
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Regression.html#xsdType_REGRESSIONNORMALIZATIONMETHOD">PMML
 * REGRESSIONNORMALIZATIONMETHOD</a>.
 *
 * For instance:
 *      - SIMPLEMAX
 *      - SOFTMAX
 *      - CAUCHIT
 *
 * See also NormalizationMethods.
 */
class NormalizationMethodType {
 public:
  enum class NormalizationMethodTypeValue { NONE, SIMPLEMAX, SOFTMAX, LOGIT, PROBIT, CLOGLOG, EXP, LOGLOG, CAUCHIT };

  NormalizationMethodTypeValue value = NormalizationMethodTypeValue::NONE;

  NormalizationMethodType() = default;

  explicit NormalizationMethodType(const std::string &value) : value(from_string(to_lower(value))) {}

  inline bool operator==(const NormalizationMethodType &other) const { return value == other.value; };

  inline bool operator!=(const NormalizationMethodType &other) const { return value != other.value; };

  static NormalizationMethodTypeValue from_string(const std::string &normalization_methodtype) {
    const static std::unordered_map<std::string, NormalizationMethodTypeValue> normalization_methodtype_converter = {
        {"none", NormalizationMethodTypeValue::NONE},       {"simplemax", NormalizationMethodTypeValue::SIMPLEMAX},
        {"softmax", NormalizationMethodTypeValue::SOFTMAX}, {"logit", NormalizationMethodTypeValue::LOGIT},
        {"probit", NormalizationMethodTypeValue::PROBIT},   {"cloglog", NormalizationMethodTypeValue::CLOGLOG},
        {"exp", NormalizationMethodTypeValue::EXP},         {"loglog", NormalizationMethodTypeValue::LOGLOG},
        {"cauchit", NormalizationMethodTypeValue::CAUCHIT}};

    try {
      return normalization_methodtype_converter.at(to_lower(normalization_methodtype));
    } catch (const std::out_of_range &exception) {
      return NormalizationMethodTypeValue::NONE;
    }
  }

  std::string to_string() const {
    switch (value) {
      case NormalizationMethodTypeValue::NONE:
        return "none";
      case NormalizationMethodTypeValue::SIMPLEMAX:
        return "simplemax";
      case NormalizationMethodTypeValue::SOFTMAX:
        return "softmax";
      case NormalizationMethodTypeValue::LOGIT:
        return "logit";
      case NormalizationMethodTypeValue::PROBIT:
        return "probit";
      case NormalizationMethodTypeValue::CLOGLOG:
        return "cloglog";
      case NormalizationMethodTypeValue::EXP:
        return "exp";
      case NormalizationMethodTypeValue::LOGLOG:
        return "loglog";
      case NormalizationMethodTypeValue::CAUCHIT:
        return "cauchit";
      default:
        return "none";
    }
  }
};

// std::ostream& operator<<(std::ostream &strm, const NormalizationMethodType
// &a) {
//    return strm << a.to_string();
//}

#endif
