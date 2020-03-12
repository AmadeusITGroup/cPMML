
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_FIELDUSAGETYPE_H
#define CPMML_FIELDUSAGETYPE_H

#include <map>
#include <string>

/**
 * @class FieldUsageType
 *
 * Class representing  <a
 * href="http://dmg.org/pmml/v4-4/MiningSchema.html#xsdType_FIELD-USAGE-TYPE">PMML
 * FieldUsageType</a>.
 *
 * It defines the usage of a field in the mining schema.
 */
class FieldUsageType {
 public:
  enum class FieldUsageTypeValue { ACTIVE, TARGET, SUPPLEMENTARY, GROUP, ORDER, FREQUENCY_WEIGHT, ANALYSIS_WEIGHT };

  FieldUsageTypeValue value = FieldUsageTypeValue::ACTIVE;

  FieldUsageType() = default;

  explicit FieldUsageType(const std::string &field_usage_type) : value(from_string(field_usage_type)) {}

  static FieldUsageTypeValue from_string(const std::string &field_usage_type) {
    const static std::unordered_map<std::string, FieldUsageTypeValue> field_usage_type_converter = {
        {"active", FieldUsageTypeValue::ACTIVE},
        {"target", FieldUsageTypeValue::TARGET},
        {"predicted", FieldUsageTypeValue::TARGET},  // before 4.1
        {"supplementary", FieldUsageTypeValue::SUPPLEMENTARY},
        {"group", FieldUsageTypeValue::GROUP},
        {"order", FieldUsageTypeValue::ORDER},
        {"frequency_weight", FieldUsageTypeValue::FREQUENCY_WEIGHT},
        {"analysis_weight", FieldUsageTypeValue::ANALYSIS_WEIGHT}};

    try {
      return field_usage_type_converter.at(to_lower(field_usage_type));
    } catch (const std::out_of_range &exception) {
      return FieldUsageTypeValue::ACTIVE;
    }
  }

  inline bool operator==(FieldUsageType other) const { return value == other.value; }
  inline bool operator==(FieldUsageTypeValue other) const { return value == other; }

  std::string to_string() const {
    switch (value) {
      case FieldUsageTypeValue::ACTIVE:
        return "ACTIVE";
      case FieldUsageTypeValue::TARGET:
        return "TARGET";
      case FieldUsageTypeValue::SUPPLEMENTARY:
        return "SUPPLEMENTARY";
      case FieldUsageTypeValue::GROUP:
        return "GROUP";
      case FieldUsageTypeValue::ORDER:
        return "ORDER";
      case FieldUsageTypeValue::FREQUENCY_WEIGHT:
        return "FREQUENCY_WEIGHT";
      case FieldUsageTypeValue::ANALYSIS_WEIGHT:
        return "ANALYSIS_WEIGHT";
      default:
        return "ACTIVE";
    }
  }
};

#endif
