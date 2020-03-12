
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_NORMDISCRETE_H
#define CPMML_NORMDISCRETE_H

#include <cstdlib>
#include <utility>

#include "expression.h"

/**
 * @class NormDiscrete
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html#xsdElement_NormDiscrete">PMML
 * NormDiscrete</a>.
 *
 * It performs the encoding of STRING values into numerical values.
 */
class NormDiscrete : public Expression {
 public:
  bool exist_missingreplacement = false;
  std::string field_name;
  size_t index = std::numeric_limits<size_t>::max();
  Value value;
  Value mapmissing_to;

  NormDiscrete() = default;

  NormDiscrete(const XmlNode &node, const unsigned int &output_index, const DataType &output_type,
               const std::shared_ptr<Indexer> &indexer)
      : Expression(output_index, output_type, indexer),
        exist_missingreplacement(node.exists_attribute("mapMissingTo")),
        field_name(node.get_attribute("field")),
        index(indexer->get_or_set(field_name)),
        value(Value(node.get_attribute("value"),
                    DataType::DataTypeValue::STRING)),  // by definition NormDiscrete is
                                                        // to map string values (to be
                                                        // checked)
        mapmissing_to(exist_missingreplacement ? Value(node.get_attribute("mapMissingTo"), output_type) : Value()) {
    inputs.insert(field_name);
  }

  inline Value eval(Sample &sample) const override {
    Value input = sample[index].value;
    if (input.missing) return mapmissing_to;
    if (input == value) return Value(1, output_type);

    return Value(0, output_type);
  }
};

#endif
