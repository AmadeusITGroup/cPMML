
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_ENSEMBLEMODEL_H
#define CPMML_ENSEMBLEMODEL_H

#include <memory>

#include "core/internal_model.h"
#include "multiplemodelmethod.h"
#include "regressionmodel/regressionmodel.h"
#include "treemodel/treemodel.h"

/**
 * @class EnsembleModel
 *
 * Implementation of InternalModel representing a <a
 * href="http://dmg.org/pmml/v4-4/MultipleModels.html">PMML MiningModel</a>.
 *
 * Through this class are represented all ensemble models. For instance, the
 * Random Forest Model or the Gradient Boosted Trees model. See also
 * MultipleModelMethod.
 */
class EnsembleModel : public InternalModel {
 public:
  Predicate predicate;
  MultipleModelMethod multiplemodelmethod;
  std::vector<Segment> ensemble;
  std::function<std::unique_ptr<InternalScore>(const Sample &)> score_ensemble;

  EnsembleModel() = default;

  EnsembleModel(const XmlNode &node, const DataDictionary &data_dictionary,
                const TransformationDictionary &transformation_dictionary, const std::shared_ptr<Indexer> &indexer)
      : InternalModel(node, data_dictionary, transformation_dictionary, indexer),
        multiplemodelmethod(node.get_child("Segmentation").get_attribute("multipleModelMethod"),
                            InternalModel::mining_function) {
    PredicateBuilder predicate_builder(indexer);
    for (const auto &child : node.get_child("Segmentation").get_childs("Segment"))
      ensemble.push_back(Segment(child, predicate_builder,
                                 build_segment_model(child, data_dictionary, InternalModel::transformation_dictionary,
                                                     predicate_builder, indexer)));

    score_ensemble = std::bind(multiplemodelmethod.function, std::placeholders::_1, ensemble);
    base_sample = create_basesample(indexer);
  };

  inline std::unique_ptr<InternalScore> score_raw(const Sample &sample) const override {
    return score_ensemble(sample);
  }

  inline std::string predict_raw(const Sample &sample) const override {
    std::unique_ptr<InternalScore> score(score_ensemble(sample));

    return score->score;
  }

  static std::unique_ptr<InternalModel> build_segment_model(const XmlNode &node, const DataDictionary &data_dictionary,
                                                            const TransformationDictionary &transformation_dictionary,
                                                            const PredicateBuilder &predicate_builder,
                                                            const std::shared_ptr<Indexer> &indexer) {
    if (node.exists_child("MiningModel")) {
      return make_unique<EnsembleModel>(node.get_child("MiningModel"), data_dictionary, transformation_dictionary,
                                        indexer);
    } else {
      if (node.exists_child("TreeModel")) {
        return make_unique<TreeModel>(node.get_child("TreeModel"), data_dictionary, predicate_builder, indexer);
      } else {
        if (node.exists_child("RegressionModel")) {
          return make_unique<RegressionModel>(node.get_child("RegressionModel"), data_dictionary, indexer);
        }
      }
    }

    throw cpmml::ParsingException("Type of model not supported in ensembles");
  }
};

#endif
