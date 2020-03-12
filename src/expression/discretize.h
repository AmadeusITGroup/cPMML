
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_DISCRETIZE_H
#define CPMML_DISCRETIZE_H

#include "expression.h"

/**
 * @class Discretize
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html#xsdElement_Discretize">PMML
 * Discretize</a>.
 *
 * It performs the discretization of numerical input fields by mapping from
 * continuous to discrete values using intervals.
 */
class Discretize : public Expression {
 public:
  XmlNode raw_node;
  bool exist_missingreplacement = false;
  Value mapmissing_to;
  std::string field_name;
  size_t index = std::numeric_limits<size_t>::max();
  DataType datatype;
  bool exist_defaultvalue = false;
  Value defaultValue;
  std::vector<Value> discretizebin_values;
  std::vector<Predicate> discretizebin_intervals;

  Discretize() = default;

  Discretize(const XmlNode &node, const size_t &output_index, const DataType &output_type,
             const std::shared_ptr<Indexer> &indexer)
      : Expression(output_index, output_type, indexer),
        raw_node(node),
        exist_missingreplacement(node.exists_attribute("mapMissingTo")),
        mapmissing_to(exist_missingreplacement ? Value(node.get_attribute("mapMissingTo"), output_type) : Value()),
        field_name(node.get_attribute("field")),
        index(indexer->get_or_set(field_name,
                                  DataType::DataTypeValue::DOUBLE)
                  .first),  // discretize makes sense only on numeric fields
        datatype(indexer->get(field_name).second),
        exist_defaultvalue(node.exists_attribute("defaultValue")),
        defaultValue(exist_defaultvalue ? Value(node.get_attribute("defaultValue"), output_type) : Value()) {
    inputs.insert(field_name);
    for (const auto &child : raw_node.get_childs("DiscretizeBin")) {
      discretizebin_values.push_back(Value(child.get_attribute("binValue"), output_type));
      discretizebin_intervals.push_back(IntervalBuilder::build(child.get_child("Interval"), index, datatype));
    }
  }

  inline Value eval(Sample &sample) const override {
    Value input = sample[index].value;

    if (input.missing) return mapmissing_to;

    for (auto i = 0u; i < discretizebin_intervals.size(); i++)
      if (discretizebin_intervals[i](input)) return discretizebin_values[i];

    return defaultValue;
  }
};

#endif
