/*******************************************************************************

 * Copyright 2019 AMADEUS. All rights reserved.

 * Author: Paolo Iannino

 *******************************************************************************/

#ifndef CPMML_SRC_EXPRESSION_PREDICTEDVALUE_H_
#define CPMML_SRC_EXPRESSION_PREDICTEDVALUE_H_

#include "outputexpression.h"

/**
 * @class PredictedValue
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Output.html#xsdType_RESULT-FEATURE">PMML
 * PredictedValue</a>.
 *
 * It is used to simply reference (and rename) the raw predicion value provided
 * by the model.
 */
class PredictedValue : public OutputExpression {
 public:
  std::string field_name;
  size_t index;

  PredictedValue() : index(std::numeric_limits<size_t>::max()) {}

  PredictedValue(const std::string &field_name, const std::shared_ptr<Indexer> &indexer, const size_t &output_index,
                 const DataType &output_type)
      : OutputExpression(output_index, output_type, indexer),
        field_name(field_name),
        index(indexer->get_or_set(field_name)) {}

  inline Value eval(Sample &sample) const override { return sample[index].value; }

  inline virtual std::string eval_str(Sample &sample, const InternalScore &score) const override {
    return score.score;
  };
};

#endif  // CPMML_SRC_EXPRESSION_PREDICTEDVALUE_H_
