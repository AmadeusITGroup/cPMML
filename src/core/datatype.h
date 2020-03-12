
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_DATATYPE_H
#define CPMML_DATATYPE_H

#include <map>
#include <string>
#include "utils/utils.h"

/**
 * @class DataType
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/DataDictionary.html#xsdType_DATATYPE">PMML
 * dataTypes</a>.
 *
 * For instance:
 *      - STRING
 *      - INTEGER
 *      - BOOLEAN
 *      - etc.
 */
class DataType {
 public:
  enum class DataTypeValue {
    STRING,
    INTEGER,
    FLOAT,
    DOUBLE,
    BOOLEAN,
  };

  DataTypeValue value;

  DataType() = default;

  DataType(const DataTypeValue &value) : value(value) {}

  DataType(const std::string &value) : value(from_string(to_lower(value))) {}

  bool operator==(const DataType &other) const { return value == other.value; };

  bool operator!=(const DataType &other) const { return value != other.value; };

  static DataTypeValue from_string(const std::string &data_type) {
    const static std::unordered_map<std::string, DataTypeValue> datatype_converter = {
        {"string", DataTypeValue::STRING},
        {"integer", DataTypeValue::DOUBLE},  // every numeric type is
                                             // treated as a double
        {"float", DataTypeValue::DOUBLE},
        {"double", DataTypeValue::DOUBLE},
        {"boolean", DataTypeValue::BOOLEAN},
    };

    try {
      return datatype_converter.at(to_lower(data_type));
    } catch (const std::out_of_range &exception) {
      throw cpmml::ParsingException("unsupported datatype: " + data_type);
    }
  }

  std::string to_string() const {
    switch (value) {
      case DataTypeValue::STRING:
        return "STRING";
      case DataTypeValue::INTEGER:
        return "INTEGER";
      case DataTypeValue::FLOAT:
        return "FLOAT";
      case DataTypeValue::DOUBLE:
        return "DOUBLE";
      case DataTypeValue::BOOLEAN:
        return "BOOLEAN";
      default:
        return "MISSING";
    }
  }
};

#endif
