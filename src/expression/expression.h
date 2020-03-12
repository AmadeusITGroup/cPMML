
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_EXPRESSION_H
#define CPMML_EXPRESSION_H

#include "core/miningschema.h"
#include "core/sample.h"
#include "core/transformationdictionary.h"
#include "core/xmlnode.h"

static const std::unordered_set<std::string> expression_names{"Constant",     "FieldRef",   "NormContinuous",
                                                              "NormDiscrete", "Discretize", "MapValues",
                                                              "TextIndex",    "Apply",      "Aggregate"};
/**
 * @class Expression
 *
 * Abstract class representing <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html#xsdGroup_EXPRESSION">PMML
 * Expression</a>.
 *
 * Its implementations provide the transformations used by DerivedField.
 */
class Expression {
 public:
  bool is_empty = true;
  size_t output_index = std::numeric_limits<size_t>::max();
  DataType output_type;
  std::shared_ptr<Indexer> indexer;
  std::set<std::string> inputs;

  Expression() = default;

  Expression(const size_t &output_index, const DataType &output_type, const std::shared_ptr<Indexer> &indexer)
      : is_empty(false), output_index(output_index), output_type(output_type), indexer(indexer) {}

  inline virtual Value eval(Sample &sample) const { return Value(); };

  Expression(const Expression &) = default;

  Expression(Expression &&) = default;

  Expression &operator=(const Expression &) = default;

  Expression &operator=(Expression &&) = default;

  virtual ~Expression() = default;
};

#endif
