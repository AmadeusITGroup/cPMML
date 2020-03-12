/*******************************************************************************

 * Copyright 2019 AMADEUS. All rights reserved.

 * Author: Paolo Iannino

 *******************************************************************************/

#ifndef CPMML_SRC_EXPRESSION_OUTPUTEXPRESSIONBUILDER_H_
#define CPMML_SRC_EXPRESSION_OUTPUTEXPRESSIONBUILDER_H_

#include <memory>

#include "expression/expression.h"
#include "expression/fieldref.h"
#include "outputexpressiontype.h"
#include "predictedValue.h"
#include "probability.h"
#include "transformedvalue.h"

/**
 * @class OutputExpressionBuilder
 *
 * Factory class to create OutputExpression objects.
 */
class OutputExpressionBuilder {
 public:
  inline static std::shared_ptr<OutputExpression> build(const XmlNode &node, const unsigned int &output_index,
                                                        const DataType &output_type,
                                                        const std::shared_ptr<Indexer> &indexer,
                                                        const std::string &model_target) {
    switch (OutputExpressionType(node.get_attribute("feature")).value) {
      case OutputExpressionType::OutputExpressionTypeValue::PREDICTED_VALUE:
        return std::make_shared<PredictedValue>(model_target, indexer, output_index, output_type);
      case OutputExpressionType::OutputExpressionTypeValue::PREDICTED_DISPLAY_VALUE:
        return std::make_shared<PredictedValue>(model_target, indexer, output_index, output_type);
      case OutputExpressionType::OutputExpressionTypeValue::TRANSFORMED_VALUE:
        return std::make_shared<TransformedValue>(node, indexer, output_index, output_type);
      case OutputExpressionType::OutputExpressionTypeValue::PROBABILITY:
        return std::make_shared<Probability>(node, indexer, output_index, output_type);
      case OutputExpressionType::OutputExpressionTypeValue::PASS_VALUE:
        return std::make_shared<PredictedValue>(model_target, indexer, output_index, output_type);
      default:
        return nullptr;
    }
  }
};

#endif  // CPMML_SRC_EXPRESSION_OUTPUTEXPRESSIONBUILDER_H_
