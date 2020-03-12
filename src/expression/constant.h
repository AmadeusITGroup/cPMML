
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_CONSTANT_H
#define CPMML_CONSTANT_H

#include "expression.h"

/**
 * @class Constant
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html#xsdElement_Constant">PMML
 * Constant</a>.
 *
 * It provides a constant value for other PMML Expressions.
 */
class Constant : public Expression {
 public:
  bool datatype_exists = false;
  Value value;

  Constant() = default;

  Constant(const XmlNode &node, const unsigned int &output_index, const DataType &output_type,
           const std::shared_ptr<Indexer> &indexer)
      : Expression(output_index, output_type, indexer),
        datatype_exists(node.exists_attribute("dataType")),
        value(datatype_exists ? Value(node.value(), node.get_attribute("dataType"))
                              : Value(node.value()))  // if datatype is missing the standard
                                                      // says the type should be inferred
  {}

  inline Value eval(Sample &sample) const override { return value; }
};

#endif
