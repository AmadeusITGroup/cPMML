
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_CLOSURE_H
#define CPMML_CLOSURE_H

#include <string>

#include "utils/utils.h"

/**
 * @class Closure
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/DataDictionary.html#interval">PMML
 * Closure</a>.
 *
 * It describes the type of boundaries for a range of continous values.
 * For instance:
 *      - closedClosed
 *      - closedOpen
 *      - etc.
 */
class Closure {
 public:
  enum class ClosureValue { CLOSED_CLOSED, OPEN_OPEN, CLOSED_OPEN, OPEN_CLOSED };

  const ClosureValue value;

  explicit Closure(const std::string &closure) : value(from_string(closure)) {}

  static ClosureValue from_string(const std::string &closure) {
    const static std::unordered_map<std::string, ClosureValue> closure_converter = {
        {"closedclosed", ClosureValue::CLOSED_CLOSED},
        {"openopen", ClosureValue::OPEN_OPEN},
        {"closedopen", ClosureValue::CLOSED_OPEN},
        {"openclosed", ClosureValue::OPEN_CLOSED}};

    try {
      return closure_converter.at(to_lower(to_lower(closure)));
    } catch (const std::out_of_range &exception) {
      throw cpmml::ParsingException(closure + " not supported");
    }
  }

  std::string to_string() const {
    switch (value) {
      case ClosureValue::CLOSED_CLOSED:
        return "CLOSED_CLOSED";
      case ClosureValue::OPEN_OPEN:
        return "OPEN_OPEN";
      case ClosureValue::CLOSED_OPEN:
        return "CLOSED_OPEN";
      case ClosureValue::OPEN_CLOSED:
        return "OPEN_CLOSED";
      default:
        return "OPEN_OPEN";
    }
  }
};

#endif
