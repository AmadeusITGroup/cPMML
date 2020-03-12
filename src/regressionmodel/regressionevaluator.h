
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_REGRESSIONEVALUATOR_H
#define CPMML_REGRESSIONEVALUATOR_H

#include <sstream>
#include <string>

#include "core/datadictionary.h"
#include "core/header.h"
#include "core/internal_evaluator.h"
#include "core/xmlnode.h"
#include "regressionmodel.h"

/**
 * @class RegressionEvaluator
 *
 * Implementation of InternalEvaluator, it is used as a wrapper of
 * RegressionModel.
 */
class RegressionEvaluator : public InternalEvaluator {
 public:
  explicit RegressionEvaluator(const XmlNode &node)
      : InternalEvaluator(node),
        regression(node.get_child("RegressionModel"), data_dictionary, transformation_dictionary, indexer){};

  RegressionModel regression;

  inline bool validate(const std::unordered_map<std::string, std::string> &sample) {
    return regression.validate(sample);
  }

  inline std::unique_ptr<InternalScore> score(const std::unordered_map<std::string, std::string> &sample) const {
    return regression.score(sample);
  }

  // Simple score, due to the type of value returned is 2/300 ns faster
  inline std::string predict(const std::unordered_map<std::string, std::string> &sample) const {
    return regression.predict(sample);
  }

  inline std::string get_target_name() const override { return regression.target_field.name; };
};

#endif
