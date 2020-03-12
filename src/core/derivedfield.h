
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_DERIVEDFIELD_H
#define CPMML_DERIVEDFIELD_H

#include "expression/expression.h"
#include "expression/expressionbuilder.h"
#include "sample.h"
#include "xmlnode.h"

/**
 * @class DerivedField
 * Class representing a <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html#xsdElement_DerivedField">PMML
 * DerivedField</a>.
 *
 * It defines a feature available to the model, which is obtained by
 * transforming other features. The associated transformation is represented by
 * the class Expression.
 *
 * @see Expression.
 */
class DerivedField {
 public:
  std::string name;
  OpType optype;
  DataType datatype;
  size_t index = std::numeric_limits<size_t>::max();
  std::shared_ptr<Expression> expression;

  DerivedField() = default;

  explicit DerivedField(const XmlNode &node, const std::shared_ptr<Indexer> &indexer)
      : name(node.get_attribute("name")),
        optype(node.get_attribute("optype")),
        datatype(node.get_attribute("dataType")),
        index(indexer->get_or_set(name, datatype).first),
        expression(ExpressionBuilder::build(node.get_child_bylist(expression_names), index, datatype, indexer)) {}

  inline void prepare(Sample &sample) const { sample.change_value(index, expression->eval(sample)); }

  inline static std::unordered_map<std::string, DerivedField> to_derivedfields(
      const std::vector<XmlNode> &nodes, const std::shared_ptr<Indexer> &indexer) {
    std::unordered_map<std::string, DerivedField> result;
    for (const auto &node : nodes)
      result.insert(std::make_pair(node.get_attribute("name"), DerivedField(node, indexer)));

    return result;
  }
};

#endif
