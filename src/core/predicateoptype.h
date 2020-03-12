
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_PREDICATEOPTYPE_H
#define CPMML_PREDICATEOPTYPE_H

/**
 * @class PredicateOpType
 *
 * Class representing the operator for the condition enforced by a predicate.
 *
 * For instance:
 * - equal
 * - notEqual
 * - isIn
 * - surrogate
 * - etc.
 */
class PredicateOpType {
 public:
  enum class PredicateOpTypeValue {
    EQUAL,
    NOT_EQUAL,
    GREATER_THAN,
    GREATER_OR_EQUAL,
    LESS_THAN,
    LESS_OR_EQUAL,
    IS_IN,
    IS_NOT_IN,
    AND,
    OR,
    XOR,
    SURROGATE,
    TRUE,
    FALSE
  };

  PredicateOpTypeValue value;

  PredicateOpType() = default;

  explicit PredicateOpType(const std::string &predicatetype) : value(from_string(predicatetype)) {}

  static PredicateOpTypeValue from_string(const std::string &predicate_type) {
    const static std::unordered_map<std::string, PredicateOpTypeValue> datatype_converter = {
        {"equal", PredicateOpTypeValue::EQUAL},
        {"notequal", PredicateOpTypeValue::NOT_EQUAL},
        {"greaterthan", PredicateOpTypeValue::GREATER_THAN},
        {"greaterorequal", PredicateOpTypeValue::GREATER_OR_EQUAL},
        {"lessthan", PredicateOpTypeValue::LESS_THAN},
        {"lessorequal", PredicateOpTypeValue::LESS_OR_EQUAL},
        {"isin", PredicateOpTypeValue::IS_IN},
        {"isnotin", PredicateOpTypeValue::IS_NOT_IN},
        {"and", PredicateOpTypeValue::AND},
        {"or", PredicateOpTypeValue::OR},
        {"xor", PredicateOpTypeValue::XOR},
        {"true", PredicateOpTypeValue::TRUE},
        {"false", PredicateOpTypeValue::FALSE},
        {"surrogate", PredicateOpTypeValue::SURROGATE}};

    try {
      return datatype_converter.at(to_lower(predicate_type));
    } catch (const std::out_of_range &exception) {
      throw cpmml::ParsingException("unsupported predicate op type: " + predicate_type);
    }
  }

  std::string to_string() const {
    switch (value) {
      case PredicateOpTypeValue::EQUAL:
        return "==";
      case PredicateOpTypeValue::NOT_EQUAL:
        return "!=";
      case PredicateOpTypeValue::GREATER_THAN:
        return ">";
      case PredicateOpTypeValue::GREATER_OR_EQUAL:
        return ">=";
      case PredicateOpTypeValue::LESS_THAN:
        return "<";
      case PredicateOpTypeValue::LESS_OR_EQUAL:
        return "<=";
      case PredicateOpTypeValue::IS_IN:
        return "isIn";
      case PredicateOpTypeValue::IS_NOT_IN:
        return "isNotIn";
      case PredicateOpTypeValue::AND:
        return "AND";
      case PredicateOpTypeValue::OR:
        return "OR";
      case PredicateOpTypeValue::XOR:
        return "XOR";
      case PredicateOpTypeValue::SURROGATE:
        return "SURROGATE";
      case PredicateOpTypeValue::TRUE:
        return "TRUE";
      case PredicateOpTypeValue::FALSE:
        return "FALSE";
      default:
        return "UNDEFINED";
    }
  }
};

#endif
