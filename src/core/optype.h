
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_OPTYPE_H
#define CPMML_OPTYPE_H

#include <string>
#include <unordered_map>

#include "utils/utils.h"

/**
 * @class OpType
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/DataDictionary.html#xsdType_OPTYPE">PMML
 * opType</a>.
 *
 * For instance:
 *      - CATEGORICAL
 *      - ORDINAL
 *      - CONTINUOUS
 */
class OpType {
 public:
  enum class OpTypeValue { CATEGORICAL, ORDINAL, CONTINUOUS, UNDEFINED };

  OpTypeValue value = OpTypeValue::UNDEFINED;

  OpType() = default;
  explicit OpType(const std::string &optype) : value(from_string(optype)) {}

  static OpTypeValue from_string(const std::string &optype) {
    const static std::unordered_map<std::string, OpTypeValue> optype_converter = {
        {"categorical", OpType::OpTypeValue::CATEGORICAL},
        {"ordinal", OpType::OpTypeValue::ORDINAL},
        {"continuous", OpType::OpTypeValue::CONTINUOUS}};

    try {
      return optype_converter.at(::to_lower(optype));
    } catch (const std::out_of_range &exception) {
      return OpTypeValue::UNDEFINED;
    }
  }

  std::string to_string() const {
    switch (value) {
      case OpTypeValue::CATEGORICAL:
        return "CATEGORICAL";
      case OpTypeValue::ORDINAL:
        return "ORDINAL";
      case OpTypeValue::CONTINUOUS:
        return "CONTINUOUS";
      case OpTypeValue::UNDEFINED:
        return "UNDEFINED";
      default:
        return "UNDEFINED";
    }
  }
};
#endif
