
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_OUTLIERTREATMENTMETHOD_H
#define CPMML_OUTLIERTREATMENTMETHOD_H

#include <map>
#include <string>
#include "utils/utils.h"

/**
 * @class OutlierTreatmentMethod
 *
 * Class representing  <a
 * href="http://dmg.org/pmml/v4-4/MiningSchema.html#xsdType_OUTLIER-TREATMENT-METHOD">PMML
 * OUTLIER-TREATMENT-METHOD</a>.
 *
 * It defines how the model should behave when encountering a feature assuming a
 * value considered as an outlier.
 */
class OutlierTreatmentMethod {
 public:
  enum class OutlierTreatmentMethodValue {
    AS_IS,
    AS_MISSING_VALUES,
    AS_EXTREME_VALUES,
  };

  OutlierTreatmentMethodValue value = OutlierTreatmentMethodValue::AS_IS;

  OutlierTreatmentMethod() = default;

  explicit OutlierTreatmentMethod(const std::string &value) : value(from_string(to_lower(value))) {}

  inline bool operator==(const OutlierTreatmentMethod &other) const { return value == other.value; };

  inline bool operator!=(const OutlierTreatmentMethod &other) const { return value != other.value; };

  static OutlierTreatmentMethodValue from_string(const std::string &outliertreatmentmethod) {
    const static std::unordered_map<std::string, OutlierTreatmentMethodValue> outliertreatmentmethod_converter = {
        {"asis", OutlierTreatmentMethodValue::AS_IS},
        {"asmissingvalues", OutlierTreatmentMethodValue::AS_MISSING_VALUES},
        {"asextremevalues", OutlierTreatmentMethodValue::AS_EXTREME_VALUES}};

    try {
      return outliertreatmentmethod_converter.at(outliertreatmentmethod);
    } catch (const std::out_of_range &exception) {
      return OutlierTreatmentMethodValue::AS_IS;
    }
  }

  std::string to_string() const {
    switch (value) {
      case OutlierTreatmentMethodValue::AS_IS:
        return "asIs";
      case OutlierTreatmentMethodValue::AS_MISSING_VALUES:
        return "asMissingValues";
      case OutlierTreatmentMethodValue::AS_EXTREME_VALUES:
        return "asExtremeValues";
      default:
        return "asIs";
    }
  }
};

#endif
