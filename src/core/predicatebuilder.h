
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_PREDICATEBUILDER_H
#define CPMML_PREDICATEBUILDER_H

#define SET_THRESHOLD 150

#include "datadictionary.h"

#include "miningschema.h"
#include "predicatetype.h"

/**
 * @class PredicateBuilder
 *
 * Factory class to create Predicate objects.
 */
class PredicateBuilder {
 public:
  std::shared_ptr<Indexer> indexer;

  PredicateBuilder() = default;

  explicit PredicateBuilder(const std::shared_ptr<Indexer> &indexer) : indexer(indexer) {}

  inline Predicate build(XmlNode node) const {
    if (node.is_empty()) return Predicate("true");

    Predicate result;
    PredicateType predicatetype(node.name());
    switch (predicatetype.value) {
      case PredicateType::PredicateTypeValue::TRUE:
        return Predicate("true");
      case PredicateType::PredicateTypeValue::FALSE:
        return Predicate("false");
      case PredicateType::PredicateTypeValue::SIMPLE:
        return Predicate(indexer->get_index(node.get_attribute("field")), node.get_attribute("operator"),
                         Value(node.get_attribute("value"), indexer->get_type(node.get_attribute("field"))));
      case PredicateType::PredicateTypeValue::SIMPLESET: {
        std::vector<std::string> values = split(node.get_child("Array").value(), " ");
        if (values.size() > SET_THRESHOLD)
          return Predicate(indexer->get_index(node.get_attribute("field")), node.get_attribute("booleanOperator"),
                           Value::createUnorderedValues(values, indexer->get_type(node.get_attribute("field"))));
        return Predicate(indexer->get_index(node.get_attribute("field")), node.get_attribute("booleanOperator"),
                         Value::createValues(values, indexer->get_type(node.get_attribute("field"))));
      }
      case PredicateType::PredicateTypeValue::COMPOUND:
        std::vector<Predicate> predicates;
        for (const auto &child : node.get_childs()) predicates.push_back(build(child));

        return Predicate(predicates, node.get_attribute("booleanOperator"));
    }

    return Predicate("true");
  }
};

#endif
