
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_SEGMENT_H
#define CPMML_SEGMENT_H

#include <memory>

#include "core/internal_model.h"
#include "core/predicatebuilder.h"
#include "ensemblemodel.h"

/**
 * @class Segment
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/MultipleModels.html#xsdElement_Segmentation">PMML
 * Segment</a>.
 *
 * Inside of an ensemble of models, this is the wrapper for the single model
 * object. See also EnsembleModel.
 */
class Segment {
 public:
  std::string id;
  double weight = 1;
  Predicate predicate;
  std::shared_ptr<InternalModel> model;

  Segment() = default;

  Segment(const XmlNode &node, const PredicateBuilder &predicate_builder, const std::shared_ptr<InternalModel> &model)
      : id(node.get_attribute("id")),
        weight(node.get_double_attribute("weight")),
        predicate(predicate_builder.build(node.get_child_bypattern("Predicate"))),
        model(model) {}

  inline std::unique_ptr<InternalScore> score(const Sample &sample) const { return model->score_raw(sample); }

  inline std::string predict(const Sample &sample) const { return model->predict_raw(sample); }
};

#endif
