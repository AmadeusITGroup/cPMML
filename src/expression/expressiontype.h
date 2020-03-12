
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_EXPRESSIONTYPE_H
#define CPMML_EXPRESSIONTYPE_H

#include <string>

#include "utils/utils.h"

/**
 * @class ExpressionType
 *
 * Class used to represents the different PMML-EXPRESSION types.
 *
 * For instance:
 * - Constant
 * - NormDiscrete
 * - MapValues
 * - etc.
 */
class ExpressionType {
 public:
  enum class ExpressionTypeValue {
    CONSTANT,
    FIELD_REF,
    NORM_CONTINUOUS,
    NORM_DISCRETE,
    DISCREZITE,
    MAP_VALUES,
    //    TEXT_INDEX,
    APPLY,
    //    AGGREGATE,
    //    UNRECOGNIZED
  };

  ExpressionTypeValue value;

  ExpressionType() = default;

  explicit ExpressionType(const std::string &value) : value(from_string(to_lower(value))) {}

  inline bool operator==(const ExpressionType &other) const { return value == other.value; };

  inline bool operator!=(const ExpressionType &other) const { return value != other.value; };

  static ExpressionTypeValue from_string(const std::string &expressiontype) {
    const static std::unordered_map<std::string, ExpressionTypeValue> expressiontype_converter = {
        {"constant", ExpressionTypeValue::CONSTANT},
        {"fieldref", ExpressionTypeValue::FIELD_REF},
        {"normcontinuous", ExpressionTypeValue::NORM_CONTINUOUS},
        {"normdiscrete", ExpressionTypeValue::NORM_DISCRETE},
        {"discretize", ExpressionTypeValue::DISCREZITE},
        {"mapvalues", ExpressionTypeValue::MAP_VALUES},
        //        {"textindex", ExpressionTypeValue::TEXT_INDEX},
        {"apply", ExpressionTypeValue::APPLY},
        //        {"aggregate", ExpressionTypeValue::AGGREGATE}
    };

    try {
      return expressiontype_converter.at(to_lower(expressiontype));
    } catch (const std::out_of_range &exception) {
      throw cpmml::ParsingException("unrecognized expression type " +
                                    expressiontype);  // cannot happen (due to how the expression type is
                                                      // retrieved (XML element name)
    }
  }

  std::string to_string() const {
    switch (value) {
      case ExpressionTypeValue::CONSTANT:
        return "Constant";
      case ExpressionTypeValue::FIELD_REF:
        return "FieldRef";
      case ExpressionTypeValue::NORM_CONTINUOUS:
        return "NormContinuous";
      case ExpressionTypeValue::NORM_DISCRETE:
        return "NormDiscrete";
      case ExpressionTypeValue::DISCREZITE:
        return "Discretize";
      case ExpressionTypeValue::MAP_VALUES:
        return "MapValues";
        //      case ExpressionTypeValue::TEXT_INDEX:
        //        return "TextIndex";
      case ExpressionTypeValue::APPLY:
        return "Apply";
        //      case ExpressionTypeValue::AGGREGATE:
        //        return "Aggregate";
      default:
        throw cpmml::ParsingException("unrecognized expression type");
    }
  }
};

#endif
