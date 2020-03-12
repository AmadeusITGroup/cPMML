/*******************************************************************************

 * Copyright 2019 AMADEUS. All rights reserved.

 * Author: Paolo Iannino

 *******************************************************************************/

#ifndef CPMML_SRC_EXPRESSION_OUTPUTEXPRESSIONTYPE_H_
#define CPMML_SRC_EXPRESSION_OUTPUTEXPRESSIONTYPE_H_

#include <string>

#include "utils/utils.h"

/**
 * @class OutputExpressionType
 *
 * Class used to represents the different PMML RESULT-FEATURE types.
 *
 * For instance:
 * - predictedValue
 * - transformedValue
 * - probability
 * - etc.
 */
class OutputExpressionType {
 public:
  enum class OutputExpressionTypeValue {
    PREDICTED_VALUE,
    PREDICTED_DISPLAY_VALUE,
    TRANSFORMED_VALUE,
    PROBABILITY,
    PASS_VALUE
  };

  OutputExpressionTypeValue value;

  OutputExpressionType() : value(OutputExpressionTypeValue::PREDICTED_VALUE) {}

  explicit OutputExpressionType(const OutputExpressionTypeValue &value) : value(value) {}

  explicit OutputExpressionType(const std::string &value) : value(from_string(to_lower(value))) {}

  inline bool operator==(const OutputExpressionType &other) const { return value == other.value; };

  inline bool operator!=(const OutputExpressionType &other) const { return value != other.value; };

  static OutputExpressionTypeValue from_string(const std::string &expressiontype) {
    const static std::unordered_map<std::string, OutputExpressionTypeValue> expressiontype_converter = {
        {"predictedvalue", OutputExpressionTypeValue::PREDICTED_VALUE},
        {"predicteddisplayvalue", OutputExpressionTypeValue::PREDICTED_DISPLAY_VALUE},
        {"transformedvalue", OutputExpressionTypeValue::TRANSFORMED_VALUE},
        {"probability", OutputExpressionTypeValue::PROBABILITY}};

    try {
      return expressiontype_converter.at(to_lower(expressiontype));
    } catch (const std::out_of_range &exception) {
      return OutputExpressionTypeValue::PASS_VALUE;
    }
  }

  std::string to_string() const {
    switch (value) {
      case OutputExpressionTypeValue::PREDICTED_VALUE:
        return "predictedValue";
      case OutputExpressionTypeValue::PREDICTED_DISPLAY_VALUE:
        return "predictedDisplayValue";
      case OutputExpressionTypeValue::TRANSFORMED_VALUE:
        return "transformedValue";
      case OutputExpressionTypeValue::PROBABILITY:
        return "probability";
      default:
        throw cpmml::ParsingException("unrecognized expression type");
    }
  }
};

#endif  // CPMML_SRC_EXPRESSION_OUTPUTEXPRESSIONTYPE_H_
