
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_ENSEMBLEEVALUATOR_H
#define CPMML_ENSEMBLEEVALUATOR_H

#include "core/internal_evaluator.h"
#include "ensemblemodel.h"

/**
 * @class EnsembleEvaluator
 *
 * Implementation of InternalEvaluator, it is used as a wrapper of
 * EnsembleModel.
 */
class EnsembleEvaluator : public InternalEvaluator {
 public:
  EnsembleModel model;

  explicit EnsembleEvaluator(const XmlNode &node)
      : InternalEvaluator(node),
        model(node.get_child("MiningModel"), data_dictionary, transformation_dictionary, indexer){};

  inline std::unique_ptr<InternalScore> score(
      const std::unordered_map<std::string, std::string> &sample) const override {
    return model.score(sample);
  }

  inline std::string predict(const std::unordered_map<std::string, std::string> &sample) const override {
    return model.predict(sample);
  }

  inline std::string get_target_name() const override { return model.target_field.name; };
};

#endif
