
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_PROPERTY_H
#define CPMML_PROPERTY_H

#include <string>

/**
 * @class Property
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/DataDictionary.html">PMML Property</a>.
 *
 * It used to define the constraints for admissible values in the
 * DataDictionary.
 */
class Property {
 public:
  enum class PropertyValue { VALID, INVALID, MISSING };

  const PropertyValue value;

  explicit Property(const std::string &property) : value(from_string(property)) {}

  static PropertyValue from_string(const std::string &property) {
    const static std::unordered_map<std::string, PropertyValue> property_converter = {
        {"valid", PropertyValue::VALID}, {"invalid", PropertyValue::INVALID}, {"missing", PropertyValue::MISSING}};

    try {
      return property_converter.at(to_lower(property));
    } catch (const std::out_of_range &exception) {
      return PropertyValue::VALID;
    }
  }

  std::string to_string() const {
    switch (value) {
      case PropertyValue::VALID:
        return "VALID";
      case PropertyValue::INVALID:
        return "INVALID";
      case PropertyValue::MISSING:
        return "MISSING";
      default:
        return "VALID";
    }
  }
};

#endif
