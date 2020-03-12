
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_DAGBUILDER_H
#define CPMML_DAGBUILDER_H

#include <string>
#include <vector>
#include "miningschema.h"
#include "transformationdictionary.h"

/**
 * @class DagBuilder
 *
 * Since there is no assumption on the ordering of DataFields and DerivedFields
 * in a PMML document. This class is used to build a directed-acyclic-graph of
 * the dependencies between fields.
 *
 * The DAG is represented as a vector where the ordering represents the order
 * through which Derived fields must be computed.
 */
class DagBuilder {
 public:
  DagBuilder() = default;

  static std::vector<std::string> build(const MiningSchema &mining_schema,
                                        const TransformationDictionary &transformation_dictionary) {
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> removed;
    std::vector<std::string> dag;

    for (const auto &derived_field : transformation_dictionary.derivedfields)
      buildR(derived_field, dag, visited, removed, mining_schema, transformation_dictionary);

    return dag;
  }

 private:
  static void buildR(const DerivedField &derived_field, std::vector<std::string> &dag,
                     std::unordered_set<std::string> &visited, std::unordered_set<std::string> &removed,
                     const MiningSchema &minining_schema, const TransformationDictionary &transformation_dictionary) {
    if (visited.find(derived_field.name) != visited.cend()) return;
    for (const auto &input : derived_field.expression->inputs) {
      if (removed.find(input) != removed.cend()) {
        removed.insert(derived_field.name);
        visited.insert(derived_field.name);
        return;
      }

      if (transformation_dictionary.contains(input)) {
        buildR(transformation_dictionary[input], dag, visited, removed, minining_schema, transformation_dictionary);
      } else if (!minining_schema.contains(input)) {
        removed.insert(derived_field.name);
        visited.insert(derived_field.name);
        return;
      }
    }

    visited.insert(derived_field.name);
    dag.push_back(derived_field.name);

    return;
  }
};

#endif
