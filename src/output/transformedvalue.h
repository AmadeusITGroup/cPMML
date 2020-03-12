/*******************************************************************************

 * Copyright 2019 AMADEUS. All rights reserved.

 * Author: Paolo Iannino

 *******************************************************************************/

#ifndef CPMML_SRC_EXPRESSION_TRANSFORMED_VALUE_H_
#define CPMML_SRC_EXPRESSION_TRANSFORMED_VALUE_H_

#include "outputexpression.h"

/**
 * @class TransformedValue
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html#xsdElement_FieldRef">PMML
 * TransformedValue</a>.
 *
 * It allows to perform user-defined transformations made of other PMML
 * Expressions or PMML Built-in functions.
 */
class TransformedValue : public OutputExpression {
 public:
  std::shared_ptr<Expression> expression;

  TransformedValue() {}

  TransformedValue(const XmlNode &node, const std::shared_ptr<Indexer> &indexer, const size_t &output_index,
                   const DataType &output_type)
      : OutputExpression(output_index, output_type, indexer),
        expression(
            ExpressionBuilder::build(node.get_child_bylist(expression_names), output_index, output_type, indexer)) {
    Expression::inputs = expression->inputs;
  }

  inline Value eval(Sample &sample) const override { return expression->eval(sample); }

  inline virtual std::string eval_str(Sample &sample, const InternalScore &score) const {
#ifndef REGEX_SUPPORT
    return std::to_string(expression->eval(sample).value);
#else
    return expression->eval(sample).svalue;
#endif
  };
};

#endif  // CPMML_SRC_EXPRESSION_TRANSFORMED_VALUE_H_
