
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_PREDICATE_H
#define CPMML_PREDICATE_H

#include "datadictionary.h"
#include "predicateoptype.h"
#include "sample.h"
#include "value.h"

/**
 * @class Predicate
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/TreeModel.html#xsdGroup_PREDICATE">PMML
 * Predicate</a>.
 *
 * It is used to enforce a constraint between values, between set of values or
 * between recursively defined set of other constraints. See also
 * PredicateOpType, TreeModel, DataDictionary.
 */
class Predicate {
 public:
  size_t feature = std::numeric_limits<size_t>::max();
  bool is_empty = true;
  PredicateOpType predicatetype = PredicateOpType("true");
  Value value;
  bool is_set_predicate = false;
  std::set<Value> values;
  bool is_hash_set = false;
  std::unordered_set<Value, Value::ValueHash> values_hash;
  bool is_compound_predicate = false;
  std::vector<Predicate> predicates;

  Predicate() = default;

  explicit Predicate(const std::string &predicatetype) : predicatetype(predicatetype) {}

  Predicate(const size_t feature, const std::string &predicatetype, const Value &value)
      : feature(feature), is_empty(false), predicatetype(predicatetype), value(value) {}

  Predicate(const size_t feature, const std::string &predicatetype, const std::set<Value> &values)
      : feature(feature), is_empty(false), predicatetype(predicatetype), is_set_predicate(true), values(values) {}

  Predicate(const size_t feature, const std::string &predicatetype,
            const std::unordered_set<Value, Value::ValueHash> &values_hash)
      : feature(feature), is_empty(false), predicatetype(predicatetype), is_hash_set(true), values_hash(values_hash) {}

  Predicate(const std::vector<Predicate> &predicates, const std::string &predicatetype)
      : is_empty(false), predicatetype(predicatetype), is_compound_predicate(true), predicates(predicates) {}

  inline bool operator()(const Sample &other) const {
    switch (predicatetype.value) {
      case PredicateOpType::PredicateOpTypeValue::TRUE:
        return true;
      case PredicateOpType::PredicateOpTypeValue::FALSE:
        return false;
      case PredicateOpType::PredicateOpTypeValue::AND:
        return AND(other);
      case PredicateOpType::PredicateOpTypeValue::OR:
        return OR(other);
      case PredicateOpType::PredicateOpTypeValue::XOR:
        return XOR(other);
      case PredicateOpType::PredicateOpTypeValue::SURROGATE:
        return SURROGATE(other);
      default:
        try {
          return operator()(other[feature].value);
        } catch (const std::out_of_range &exception) {
#ifdef DEBUG
          throw cpmml::GenericException("feature \"" + other[feature].name +
                                        "\" not found in sample: " + other.to_string());
#else
          throw cpmml::MissingValueException("missing feature in sample");
#endif
        }
    }
  }

  inline bool operator()(const Value &other) const {
    switch (predicatetype.value) {
      case PredicateOpType::PredicateOpTypeValue::TRUE:
        return true;
      case PredicateOpType::PredicateOpTypeValue::FALSE:
        return false;
      case PredicateOpType::PredicateOpTypeValue::EQUAL:
        return other == value;
      case PredicateOpType::PredicateOpTypeValue::NOT_EQUAL:
        return other != value;
      case PredicateOpType::PredicateOpTypeValue::GREATER_THAN:
        return other > value;
      case PredicateOpType::PredicateOpTypeValue::GREATER_OR_EQUAL:
        return other >= value;
      case PredicateOpType::PredicateOpTypeValue::LESS_THAN:
        return other < value;
      case PredicateOpType::PredicateOpTypeValue::LESS_OR_EQUAL:
        return other <= value;
      case PredicateOpType::PredicateOpTypeValue::IS_IN: {
        if (is_hash_set) return other.is_in(values_hash);
        return other.is_in(values);
      }
      case PredicateOpType::PredicateOpTypeValue::IS_NOT_IN: {
        if (is_hash_set) return other.is_not_in(values_hash);
        return other.is_not_in(values);
      }
      case PredicateOpType::PredicateOpTypeValue::AND:
        return AND(other);
      case PredicateOpType::PredicateOpTypeValue::OR:
        return OR(other);
      case PredicateOpType::PredicateOpTypeValue::XOR:
        return XOR(other);
      case PredicateOpType::PredicateOpTypeValue::SURROGATE:
        return SURROGATE(other);
    }

    return false;
  }

  inline bool AND(const Sample &other) const {
    for (auto it = predicates.cbegin(); it != predicates.cend(); it++)
      if (!it->operator()(other)) return false;

    return true;
  }

  inline bool OR(const Sample &other) const {
    for (auto it = predicates.cbegin(); it != predicates.cend(); it++)
      if (it->operator()(other)) return true;

    return false;
  }

  inline bool XOR(const Sample &other) const {
    bool first = predicates.front().operator()(other);

    for (auto it = predicates.cbegin() + 1; it != predicates.cend(); it++)
      if (it->operator()(other) != first) return true;

    return false;
  }

  inline bool SURROGATE(const Sample &other) const {
    bool result = false;

    for (auto it = predicates.cbegin(); it != predicates.cend(); it++) {
      result = it->operator()(other);
      if (result) return true;
    }

    return result;
  }

  inline bool AND(const Value &other) const {
    for (auto it = predicates.cbegin(); it != predicates.cend(); it++)
      if (!it->operator()(other)) return false;

    return true;
  }

  inline bool OR(const Value &other) const {
    for (auto it = predicates.cbegin(); it != predicates.cend(); it++)
      if (it->operator()(other)) return true;

    return false;
  }

  inline bool XOR(const Value &other) const {
    bool first = predicates.front().operator()(other);

    for (auto it = predicates.cbegin() + 1; it != predicates.cend(); it++)
      if (it->operator()(other) != first) return true;

    return false;
  }

  inline bool SURROGATE(const Value &other) const {
    bool result = false;

    for (auto it = predicates.cbegin(); it != predicates.cend(); it++) {
      result = it->operator()(other);
      if (result) return true;
    }

    return result;
  }
};

#endif
