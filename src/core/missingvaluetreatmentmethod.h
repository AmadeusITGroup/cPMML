
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_MISSINGVALUETREATMENTMETHOD_H
#define CPMML_MISSINGVALUETREATMENTMETHOD_H

#include <map>
#include <string>
#include "utils/utils.h"

/**
 * @class MissingValueTreatmentMethod
 *
 * Class representing  <a
 * href="http://dmg.org/pmml/v4-4/MiningSchema.html#xsdType_MISSING-VALUE-TREATMENT-METHOD">PMML
 * MISSING-VALUE-TREATMENT-METHOD</a>.
 *
 * It defines how the model should behave when encountering a feature whose
 * value is missing.
 */
class MissingValueTreatmentMethod {
 public:
  enum class MissingValueTreatmentMethodValue { AS_IS, AS_MEAN, AS_MODE, AS_MEDIAN, AS_VALUE };

  MissingValueTreatmentMethodValue value = MissingValueTreatmentMethodValue::AS_IS;

  MissingValueTreatmentMethod() = default;

  explicit MissingValueTreatmentMethod(const std::string &value) : value(from_string(to_lower(value))) {}

  inline bool operator==(const MissingValueTreatmentMethod &other) const { return value == other.value; };

  inline bool operator!=(const MissingValueTreatmentMethod &other) const { return value != other.value; };

  static MissingValueTreatmentMethodValue from_string(const std::string &missingvaluetreatmentmethod) {
    const static std::unordered_map<std::string, MissingValueTreatmentMethodValue>
        missingvaluetreatmentmethod_converter = {{"asis", MissingValueTreatmentMethodValue::AS_IS},
                                                 {"asmean", MissingValueTreatmentMethodValue::AS_MEAN},
                                                 {"asmode", MissingValueTreatmentMethodValue::AS_MODE},
                                                 {"asmedian", MissingValueTreatmentMethodValue::AS_MEDIAN},
                                                 {"asvalue", MissingValueTreatmentMethodValue::AS_VALUE}};

    try {
      return missingvaluetreatmentmethod_converter.at(to_lower(missingvaluetreatmentmethod));
    } catch (const std::out_of_range &exception) {
      return MissingValueTreatmentMethodValue::AS_IS;
    }
  }

  std::string to_string() const {
    switch (value) {
      case MissingValueTreatmentMethodValue::AS_IS:
        return "asIs";
      case MissingValueTreatmentMethodValue::AS_MEAN:
        return "asMean";
      case MissingValueTreatmentMethodValue::AS_MODE:
        return "asMode";
      case MissingValueTreatmentMethodValue::AS_MEDIAN:
        return "asMedian";
      case MissingValueTreatmentMethodValue::AS_VALUE:
        return "asValue";
      default:
        return "asIs";
    }
  }
};

#endif
