/*******************************************************************************

 * Copyright 2019 AMADEUS. All rights reserved.

 * Author: Paolo Iannino

 *******************************************************************************/

#ifndef CPMML_SRC_EXPRESSION_OUTPUTEXPRESSION_H_
#define CPMML_SRC_EXPRESSION_OUTPUTEXPRESSION_H_

#include "core/internal_score.h"
#include "core/miningschema.h"
#include "core/sample.h"
#include "core/transformationdictionary.h"
#include "core/xmlnode.h"

/**
 * @class OutputExpression
 *
 * Abstract class representing <a
 * href="http://dmg.org/pmml/v4-4/Output.html#xsdType_RESULT-FEATURE">PMML
 * RESULT-FEATURE</a>.
 *
 * Its implementations provide the transformations used by OutputField.
 */
class OutputExpression : public Expression {
 public:
  OutputExpression() {}

  OutputExpression(const size_t &output_index, const DataType &output_type, const std::shared_ptr<Indexer> &indexer)
      : Expression(output_index, output_type, indexer) {}

  inline virtual Value eval(Sample &sample) const override { return Value(); };

  inline virtual std::string eval_str(Sample &sample, const InternalScore &score) const { return ""; };

  inline virtual double eval_double(Sample &sample, const InternalScore &score) const { return double_min(); };

  OutputExpression(const OutputExpression &) = default;

  OutputExpression(OutputExpression &&) = default;

  OutputExpression &operator=(const OutputExpression &) = default;

  OutputExpression &operator=(OutputExpression &&) = default;

  virtual ~OutputExpression() = default;
};

#endif  // CPMML_SRC_EXPRESSION_OUTPUTEXPRESSION_H_
