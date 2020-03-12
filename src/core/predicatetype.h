
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_PREDICATETYPE_H
#define CPMML_PREDICATETYPE_H

/**
 * @class PredicateType
 *
 * Class representing the type of predicate.
 *
 * For instance:
 * - SimplePredicate
 * - SimpleSetPredicate
 * - CompoundPredicate
 * - True
 * - False
 *
 * See also <a
 * href="http://dmg.org/pmml/v4-4/TreeModel.html#xsdGroup_PREDICATE">PMML
 * Predicate</a>.
 */
class PredicateType {
 public:
  enum class PredicateTypeValue { TRUE, FALSE, SIMPLE, SIMPLESET, COMPOUND };

  PredicateTypeValue value;

  PredicateType() = default;

  explicit PredicateType(const std::string &predicatetype) : value(from_string(predicatetype)) {}

  static PredicateTypeValue from_string(const std::string &predicate_type) {
    const static std::unordered_map<std::string, PredicateTypeValue> predicatetype_converter = {
        {"true", PredicateTypeValue::TRUE},
        {"false", PredicateTypeValue::FALSE},
        {"simplepredicate", PredicateTypeValue::SIMPLE},
        {"simplesetpredicate", PredicateTypeValue::SIMPLESET},
        {"compoundpredicate", PredicateTypeValue::COMPOUND}};

    try {
      return predicatetype_converter.at(to_lower(predicate_type));
    } catch (const std::out_of_range &exception) {
      throw cpmml::ParsingException("unsupported predicate type: " + predicate_type);
    }
  }

  std::string to_string() const {
    switch (value) {
      case PredicateTypeValue::TRUE:
        return "True";
      case PredicateTypeValue::FALSE:
        return "False";
      case PredicateTypeValue::SIMPLE:
        return "SimplePredicate";
      case PredicateTypeValue::SIMPLESET:
        return "SimpleSetPredicate";
      case PredicateTypeValue::COMPOUND:
        return "CompoundPredicate";
      default:
        return "UNDEFINED";
    }
  }
};

#endif
