
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_EXPRESSIONBUILDER_H
#define CPMML_EXPRESSIONBUILDER_H

#include <memory>

#include "apply.h"
#include "constant.h"
#include "discretize.h"
#include "expression.h"
#include "expressiontype.h"
#include "fieldref.h"
#include "mapvalues.h"
#include "normcontinuous.h"
#include "normdiscrete.h"

/**
 * @class ExpressionBuilder
 *
 * Factory class to create Expression objects.
 */
class ExpressionBuilder {
 public:
  inline static std::shared_ptr<Expression> build(const XmlNode &node, const unsigned int &output_index,
                                                  const DataType &output_type,
                                                  const std::shared_ptr<Indexer> &indexer) {
    switch (ExpressionType(node.name()).value) {
      case ExpressionType::ExpressionTypeValue::CONSTANT:
        return std::make_shared<Constant>(node, output_index, output_type,
                                          indexer);  // to be refined in case of Parameter fields
      case ExpressionType::ExpressionTypeValue::FIELD_REF:
        return std::make_shared<FieldRef>(node, output_index, output_type, indexer);
      case ExpressionType::ExpressionTypeValue::NORM_CONTINUOUS:
        return std::make_shared<NormContinuous>(node, output_index, output_type, indexer);
      case ExpressionType::ExpressionTypeValue::NORM_DISCRETE:
        return std::make_shared<NormDiscrete>(node, output_index, output_type, indexer);
      case ExpressionType::ExpressionTypeValue::DISCREZITE:
        return std::make_shared<Discretize>(node, output_index, output_type, indexer);
      case ExpressionType::ExpressionTypeValue::APPLY:
        return std::make_shared<Apply>(node, output_index, output_type, indexer);
      case ExpressionType::ExpressionTypeValue::MAP_VALUES:
        return std::make_shared<MapValues>(node, output_index, output_type, indexer);
      default:
        return nullptr;
    }
  }
};

#endif
