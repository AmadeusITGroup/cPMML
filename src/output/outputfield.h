
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_OUTPUTFIELD_H
#define CPMML_OUTPUTFIELD_H

#include "core/xmlnode.h"
#include "expression/expression.h"
#include "outputexpressionbuilder.h"
#include "outputexpressiontype.h"

/**
 * @class OutputField
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Output.html#xsdElement_OutputField">PMML
 * OutputField</a>.
 *
 * It defines a which output features the model will produce from the raw
 * prediction.
 */
class OutputField {
 public:
  std::string name;
  OpType optype;
  //  std::string target_field; // needed in case of multiple outputs, not
  //  supported by choice
  bool derived;
  DataType datatype;
  size_t index;
  std::shared_ptr<OutputExpression> expression;

  OutputField() : derived(false), index(std::numeric_limits<size_t>::max()) {}

  OutputField(const XmlNode &node, const std::shared_ptr<Indexer> &indexer, const std::string &model_target)
      : name(node.get_attribute("name")),
        optype(node.get_attribute("optype")),
        derived(OutputExpressionType(node.get_attribute("feature")).value ==
                OutputExpressionType::OutputExpressionTypeValue::TRANSFORMED_VALUE)
  //      target_field(node.get_attribute("targetField")),
  {
    if (!node.exists_attribute("dataType")) {
      if (indexer->contains(model_target))
        datatype = indexer->get_type(model_target);
      else
        throw cpmml::ParsingException("Impossible to determine datatype for output: " + name);
    } else
      datatype = node.get_attribute("dataType");

    index = indexer->get_or_set(name, datatype).first;
    expression = OutputExpressionBuilder::build(node, index, datatype, indexer, model_target);
  }

  inline void prepare(Sample &sample) const { sample.change_value_if_missing(index, expression->eval(sample)); }

  inline void add_output(Sample &sample, InternalScore &score) const {
    switch (datatype.value) {
      case DataType::DataTypeValue::STRING:
        score.str_outputs[name] = expression->eval_str(sample, score);
        break;
      default:
        score.num_outputs[name] = expression->eval_double(sample, score);
    }
  }

  inline static std::unordered_map<std::string, OutputField> to_outputfields(const std::vector<XmlNode> &nodes,
                                                                             std::shared_ptr<Indexer> indexer,
                                                                             const std::string &model_target) {
    std::unordered_map<std::string, OutputField> result;
    for (const auto &node : nodes) {
      OutputField derived_field(node, indexer, model_target);
      result.insert(std::make_pair(derived_field.name, derived_field));
    }

    return result;
  }
};

#endif
