
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_FASTPREDICATE_H
#define CPMML_FASTPREDICATE_H

#include "predicate.h"
#include "predicateoptype.h"
#include "sample.h"

constexpr bool _false(const Sample &other) { return false; }

constexpr bool _true(const Sample &other) { return true; }

inline bool equal(const Sample &other, const Value &value, const size_t &feature) {
  return other[feature].cvalue() == value;
}

inline bool not_equal(const Sample &other, const Value &value, const size_t &feature) {
  return other[feature].cvalue() != value;
}

inline bool greater_than(const Sample &other, const Value &value, const size_t &feature) {
  return other[feature].cvalue() > value;
}

inline bool greater_orequal(const Sample &other, const Value &value, const size_t &feature) {
  return other[feature].cvalue() >= value;
}

inline bool less_than(const Sample &other, const Value &value, const size_t &feature) {
  return other[feature].cvalue() < value;
}

inline bool less_orequal(const Sample &other, const Value &value, const size_t &feature) {
  return other[feature].cvalue() <= value;
}

inline bool is_in(const Sample &other, const std::set<Value> &values, const size_t &feature) {
  return other[feature].cvalue().is_in(values);
}

inline bool is_notin(const Sample &other, const std::set<Value> &values, const size_t &feature) {
  return other[feature].cvalue().is_not_in(values);
}
// inline bool is_in (const Sample& other, const std::unordered_set<Value,
// Value::ValueHash>& values, const size_t& feature){return
// other[feature].value.is_in(values);} inline bool is_notin (const
// Sample& other, const std::unordered_set<Value, Value::ValueHash>& values,
// const size_t& feature){return other[feature].value.is_not_in(values);}
inline bool _and(const Sample &other, const std::vector<std::function<bool(const Sample &)>> &predicates) {
  for (const auto &predicate : predicates)
    if (!predicate(other)) return false;

  return true;
}

inline bool _or(const Sample &other, const std::vector<std::function<bool(const Sample &)>> &predicates) {
  for (const auto &predicate : predicates)
    if (predicate(other)) return true;

  return false;
}

inline bool _xor(const Sample &other, const std::vector<std::function<bool(const Sample &)>> &predicates) {
  bool first = predicates.front()(other);

  for (auto it = predicates.cbegin() + 1; it != predicates.cend(); it++)
    if (it->operator()(other) != first) return true;

  return false;
}

inline bool surrogate(const Sample &other, const std::vector<std::function<bool(const Sample &)>> &predicates) {
  for (const auto &predicate : predicates) {
    try {
      return predicate(other);
    } catch (const cpmml::Exception &e) {
    }
  }

  return false;
}

inline std::vector<std::function<bool(const Sample &)>> to_function(const std::vector<Predicate> &predicates);

inline std::function<bool(const Sample &)> to_function(const Predicate &predicate) {
  switch (predicate.predicatetype.value) {
    case PredicateOpType::PredicateOpTypeValue::TRUE:
      return std::bind(_true, std::placeholders::_1);
    case PredicateOpType::PredicateOpTypeValue::FALSE:
      return std::bind(_false, std::placeholders::_1);
    case PredicateOpType::PredicateOpTypeValue::EQUAL:
      return std::bind(equal, std::placeholders::_1, predicate.value, predicate.feature);
    case PredicateOpType::PredicateOpTypeValue::NOT_EQUAL:
      return std::bind(not_equal, std::placeholders::_1, predicate.value, predicate.feature);
    case PredicateOpType::PredicateOpTypeValue::GREATER_THAN:
      return std::bind(greater_than, std::placeholders::_1, predicate.value, predicate.feature);
    case PredicateOpType::PredicateOpTypeValue::GREATER_OR_EQUAL:
      return std::bind(greater_orequal, std::placeholders::_1, predicate.value, predicate.feature);
    case PredicateOpType::PredicateOpTypeValue::LESS_THAN:
      return std::bind(less_than, std::placeholders::_1, predicate.value, predicate.feature);
    case PredicateOpType::PredicateOpTypeValue::LESS_OR_EQUAL:
      return std::bind(less_orequal, std::placeholders::_1, predicate.value, predicate.feature);
    case PredicateOpType::PredicateOpTypeValue::IS_IN:
      return std::bind(is_in, std::placeholders::_1, predicate.values, predicate.feature);
    case PredicateOpType::PredicateOpTypeValue::IS_NOT_IN:
      return std::bind(is_notin, std::placeholders::_1, predicate.values, predicate.feature);
    case PredicateOpType::PredicateOpTypeValue::AND:
      return std::bind(_and, std::placeholders::_1, to_function(predicate.predicates));
    case PredicateOpType::PredicateOpTypeValue::OR:
      return std::bind(_or, std::placeholders::_1, to_function(predicate.predicates));
    case PredicateOpType::PredicateOpTypeValue::XOR:
      return std::bind(_xor, std::placeholders::_1, to_function(predicate.predicates));
    case PredicateOpType::PredicateOpTypeValue::SURROGATE:
      return std::bind(surrogate, std::placeholders::_1, to_function(predicate.predicates));
  }

  return std::bind(_false, std::placeholders::_1);
}

inline std::vector<std::function<bool(const Sample &)>> to_function(const std::vector<Predicate> &predicates) {
  std::vector<std::function<bool(const Sample &)>> result;

  for (const auto &predicate : predicates) result.push_back(to_function(predicate));

  return result;
}

#endif
