
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_INVALIDVALUETREATMENTMETHOD_H
#define CPMML_INVALIDVALUETREATMENTMETHOD_H

#include <map>
#include <string>
#include "utils/utils.h"

/**
 * @class InvalidValueTreatmentMethod
 *
 * Class representing  <a
 * href="http://dmg.org/pmml/v4-4/MiningSchema.html#xsdType_INVALID-VALUE-TREATMENT-METHOD">PMML
 * INVALID-VALUE-TREATMENT-METHOD</a>.
 *
 * It defines how the model should behave when encountering a feature assuming a
 * value not respecting the constraints defined in the DataDictionary.
 */
class InvalidValueTreatmentMethod {
 public:
  enum class InvalidValueTreatmentMethodValue { RETURN_INVALID, AS_IS, AS_MISSING };

  InvalidValueTreatmentMethodValue value = InvalidValueTreatmentMethodValue::RETURN_INVALID;

  InvalidValueTreatmentMethod() = default;

  explicit InvalidValueTreatmentMethod(const std::string &value) : value(from_string(value)) {}

  inline bool operator==(const InvalidValueTreatmentMethod &other) const { return value == other.value; };

  inline bool operator!=(const InvalidValueTreatmentMethod &other) const { return value != other.value; };

  static InvalidValueTreatmentMethodValue from_string(const std::string &invalidvaluetreatmentmethod) {
    const static std::unordered_map<std::string, InvalidValueTreatmentMethodValue>
        invalidvaluetreatmentmethod_converter = {{"asis", InvalidValueTreatmentMethodValue::AS_IS},
                                                 {"returninvalid", InvalidValueTreatmentMethodValue::RETURN_INVALID},
                                                 {"asmissing", InvalidValueTreatmentMethodValue::AS_MISSING}};

    try {
      return invalidvaluetreatmentmethod_converter.at(to_lower(invalidvaluetreatmentmethod));
    } catch (const std::out_of_range &exception) {
      return InvalidValueTreatmentMethodValue::RETURN_INVALID;
    }
  }

  std::string to_string() const {
    switch (value) {
      case InvalidValueTreatmentMethodValue::AS_IS:
        return "asIs";
      case InvalidValueTreatmentMethodValue::RETURN_INVALID:
        return "returnInvalid";
      case InvalidValueTreatmentMethodValue::AS_MISSING:
        return "asMissing";
      default:
        return "returnInvalid";
    }
  }
};

#endif
