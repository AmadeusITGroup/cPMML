
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_BUILTINFUNCTIONS_H
#define CPMML_BUILTINFUNCTIONS_H

#include <cmath>

#include "cPMML.h"
#include "value.h"

/**
 * @class BuiltInFunction
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/BuiltinFunctions.html">PMML built-in
 * functions</a>.
 *
 * It defines a set of predefined functions implementing low-level operations.
 * For instance:
 *      - arithmetic functions: min, max, sum, etc.
 *      - boolean functions: equal, isMissing, etc.
 *      - etc.
 */
class BuiltInFunction {  // inputs are mapped by position
 public:
  enum class BuiltInFunctionType {
    PLUS,
    MINUS,
    MUL,
    DIV,
    MAX,
    MIN,
    SUM,
    AVG,
    EXP,
    IS_MISSING,
    IS_NOT_MISSING,
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    LESS_THAN_EQUAL,
    GREATER_THAN,
    GREATER_THAN_EQUAL,
    IS_IN,
    IS_NOT_IN,
#ifdef REGEX_SUPPORT
    REPLACE,
#endif
    IDENTITY  // introduced to have "empty" function type
  };

  std::string function_string;
  BuiltInFunctionType function_type = BuiltInFunctionType::IDENTITY;
  int n_args = -1;
  std::function<Value(const std::vector<Value> &)> function;

  BuiltInFunction() = default;

  explicit BuiltInFunction(const std::string &function_string)
      : function_string(function_string),
        function_type(from_string(function_string)),
        n_args(get_nargs(function_type)),
        function(get_function(function_type)) {}

  static BuiltInFunctionType from_string(const std::string &builtinfunction) {
    const static std::unordered_map<std::string, BuiltInFunctionType> builtinfunction_converter = {
        {"+", BuiltInFunctionType::PLUS},
        {"-", BuiltInFunctionType::MINUS},
        {"*", BuiltInFunctionType::MUL},
        {"/", BuiltInFunctionType::DIV},
        {"max", BuiltInFunctionType::MAX},
        {"min", BuiltInFunctionType::MIN},
        {"sum", BuiltInFunctionType::SUM},
        {"avg", BuiltInFunctionType::AVG},
        {"exp", BuiltInFunctionType::EXP},
        {"ismissing", BuiltInFunctionType::IS_MISSING},
        {"isnotmissing", BuiltInFunctionType::IS_NOT_MISSING},
        {"equal", BuiltInFunctionType::EQUAL},
        {"notequal", BuiltInFunctionType::NOT_EQUAL},
        {"lessthan", BuiltInFunctionType::LESS_THAN},
        {"lessorequal", BuiltInFunctionType::LESS_THAN_EQUAL},
        {"greaterthan", BuiltInFunctionType::GREATER_THAN},
        {"greaterorequal", BuiltInFunctionType::GREATER_THAN_EQUAL},
        {"isin", BuiltInFunctionType::IS_IN},
        {"isnotin", BuiltInFunctionType::IS_NOT_IN},
#ifdef REGEX_SUPPORT
        {"replace", BuiltInFunctionType::REPLACE}
#endif
    };

    try {
      return builtinfunction_converter.at(to_lower(builtinfunction));
    } catch (const std::out_of_range &exception) {
      throw cpmml::ParsingException(builtinfunction + " not supported");
    }
  }

  static int get_nargs(const BuiltInFunctionType &function_type) {
    switch (function_type) {
      case BuiltInFunctionType::PLUS:
        return 2;
      case BuiltInFunctionType::MINUS:
        return 2;
      case BuiltInFunctionType::MUL:
        return 2;
      case BuiltInFunctionType::DIV:
        return 2;
      case BuiltInFunctionType::IS_MISSING:
        return 1;
      case BuiltInFunctionType::IS_NOT_MISSING:
        return 1;
      case BuiltInFunctionType::IS_IN:
        return 1;
      case BuiltInFunctionType::IS_NOT_IN:
        return 1;
#ifdef REGEX_SUPPORT
      case BuiltInFunctionType::REPLACE:
        return 3;
#endif
      default:
        return -1;
    }
  }

  static std::function<Value(const std::vector<Value> &)> get_function(const BuiltInFunctionType &function_type) {
    switch (function_type) {
      case BuiltInFunctionType::PLUS:
        return plus;
      case BuiltInFunctionType::MINUS:
        return minus;
      case BuiltInFunctionType::MUL:
        return mul;
      case BuiltInFunctionType::DIV:
        return div;
      case BuiltInFunctionType::MAX:
        return max;
      case BuiltInFunctionType::MIN:
        return min;
      case BuiltInFunctionType::SUM:
        return sum;
      case BuiltInFunctionType::AVG:
        return avg;
      case BuiltInFunctionType::EXP:
        return exp;
      case BuiltInFunctionType::IS_MISSING:
        return is_missing;
      case BuiltInFunctionType::IS_NOT_MISSING:
        return is_notmissing;
      case BuiltInFunctionType::IS_IN:
        return is_in;
      case BuiltInFunctionType::IS_NOT_IN:
        return is_notin;
      case BuiltInFunctionType::EQUAL:
        return equal;
      case BuiltInFunctionType::NOT_EQUAL:
        return not_equal;
      case BuiltInFunctionType::LESS_THAN:
        return less_than;
      case BuiltInFunctionType::LESS_THAN_EQUAL:
        return less_thanorequal;
      case BuiltInFunctionType::GREATER_THAN:
        return greater_than;
      case BuiltInFunctionType::GREATER_THAN_EQUAL:
        return greater_thanorequal;
#ifdef REGEX_SUPPORT
      case BuiltInFunctionType::REPLACE:
        return replace;
#endif
      default:
        throw cpmml::ParsingException("unsupported function");
    }
  }

  inline Value operator()(const std::vector<Value> &input) const {
    if (n_args != -1 && n_args != (int)input.size()) throw cpmml::InvalidValueException("Wrong number of inputs");
    return function(input);
  }

  inline static Value plus(const std::vector<Value> &input) { return input[0] + input[1]; }
  inline static Value minus(const std::vector<Value> &input) { return input[0] - input[1]; }
  inline static Value mul(const std::vector<Value> &input) { return input[0] * input[1]; }
  inline static Value div(const std::vector<Value> &input) { return input[0] / input[1]; }
  inline static Value max(const std::vector<Value> &input) { return Value::max(input); }
  inline static Value min(const std::vector<Value> &input) { return Value::min(input); }
  inline static Value sum(const std::vector<Value> &input) { return Value::sum(input); }
  inline static Value avg(const std::vector<Value> &input) { return Value::sum(input) / Value(input.size()); }
  inline static Value is_missing(const std::vector<Value> &input) {
    return Value(input[0].missing, DataType::DataTypeValue::BOOLEAN);
  }
  inline static Value is_notmissing(const std::vector<Value> &input) {
    return Value(!input[0].missing, DataType::DataTypeValue::BOOLEAN);
  }
  inline static Value equal(const std::vector<Value> &input) {
    return Value(input[0] == input[1], DataType::DataTypeValue::BOOLEAN);
  }
  inline static Value not_equal(const std::vector<Value> &input) {
    return Value(input[0] != input[1], DataType::DataTypeValue::BOOLEAN);
  }
  inline static Value less_than(const std::vector<Value> &input) {
    return Value(input[0] < input[1], DataType::DataTypeValue::BOOLEAN);
  }
  inline static Value less_thanorequal(const std::vector<Value> &input) {
    return Value(input[0] <= input[1], DataType::DataTypeValue::BOOLEAN);
  }
  inline static Value greater_than(const std::vector<Value> &input) {
    return Value(input[0] > input[1], DataType::DataTypeValue::BOOLEAN);
  }
  inline static Value greater_thanorequal(const std::vector<Value> &input) {
    return Value(input[0] >= input[1], DataType::DataTypeValue::BOOLEAN);
  }
  inline static Value exp(const std::vector<Value> &input) {
    return Value(std::exp(input[0].value), DataType::DataTypeValue::DOUBLE);
  }
  inline static Value is_in(const std::vector<Value> &input) {
    auto found = std::find(input.begin() + 1, input.end(), input[0]);
    return Value(found != input.begin() && found != input.end(), DataType::DataTypeValue::BOOLEAN);
  }
  inline static Value is_notin(const std::vector<Value> &input) {
    return Value(!(is_in(input).value), DataType::DataTypeValue::BOOLEAN);
  }

#ifdef REGEX_SUPPORT
  inline static Value replace(const std::vector<Value> &input) {
    return input[0].replace(input[1].svalue, input[2].svalue);
  }
#endif
};

#endif
