
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_TREEEVALUATOR_H
#define CPMML_TREEEVALUATOR_H

#include <sstream>
#include <string>

#include "core/datadictionary.h"
#include "core/header.h"
#include "core/internal_evaluator.h"
#include "core/xmlnode.h"
#include "treemodel.h"

/**
 * @class TreeEvaluator
 *
 * Implementation of InternalEvaluator, it is used as a wrapper of TreeModel.
 */
class TreeEvaluator : public InternalEvaluator {
 public:
  explicit TreeEvaluator(const XmlNode &node)
      : InternalEvaluator(node),
        tree(node.get_child("TreeModel"), data_dictionary, transformation_dictionary, indexer){};

  TreeModel tree;

  inline bool validate(const std::unordered_map<std::string, std::string> &sample) { return tree.validate(sample); }

  inline std::unique_ptr<InternalScore> score(
      const std::unordered_map<std::string, std::string> &sample) const override {
    return tree.score(sample);
  }

  // Simple score, due to the type of value returned is 2/300 ns faster
  inline std::string predict(const std::unordered_map<std::string, std::string> &sample) const {
    return tree.predict(sample);
  }

  inline std::string get_target_name() const override { return tree.target_field.name; };
};

#endif
