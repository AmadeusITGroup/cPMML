
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_MAPVALUES_H
#define CPMML_MAPVALUES_H

#include "expression.h"
#include "treetable/treetable.h"

/**
 * @class MapValues
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html#xsdElement_MapValues">PMML
 * MapValues</a>.
 *
 * Through the use of a table (implemented with TreeTable) it maps discrete
 * values to other discrete values.
 */
class MapValues : public Expression {
 public:
  XmlNode raw_node;
  bool exist_missingreplacement = false;
  Value mapmissing_to;
  bool exist_defaultvalue = false;
  Value defaultValue;
  std::string out;
  std::unordered_map<std::string, size_t> index;
  std::unordered_map<std::string, DataType> datatype;
  std::vector<std::string> columns;
  TreeTable<Value, Value, Value::ValueHash> tree_table;
  std::unordered_map<std::string, std::string> fieldcolumn_pairs;
  std::vector<size_t> raw_columns;

  MapValues() = default;

  MapValues(const XmlNode &node, const size_t &output_index, const DataType &output_type,
            const std::shared_ptr<Indexer> &indexer)
      : Expression(output_index, output_type, indexer),
        raw_node(node),
        exist_missingreplacement(node.exists_attribute("mapMissingTo")),
        mapmissing_to(exist_missingreplacement ? Value(node.get_attribute("mapMissingTo"), output_type) : Value()),
        exist_defaultvalue(node.exists_attribute("defaultValue")),
        defaultValue(exist_defaultvalue ? Value(node.get_attribute("defaultValue"), output_type) : Value()),
        out(node.get_attribute("outputColumn")) {
    std::string tmp_column;
    std::string tmp_input;
    for (const auto &n : node.get_childs("FieldColumnPair")) {
      tmp_input = n.get_attribute("field");
      inputs.insert(tmp_input);
      tmp_column = n.get_attribute("column");
      columns.push_back(tmp_column);
      fieldcolumn_pairs[tmp_column] = tmp_input;
    }

    for (const auto &column : columns) raw_columns.push_back(indexer->get_or_set(fieldcolumn_pairs[column]));

    for (const auto &n : raw_node.get_child("InlineTable").get_childs("row")) {
      std::vector<Value> keys;
      for (const auto &column : columns) {
        keys.push_back(Value(n.get_child(column).value()));  // datatype is inferred!!!
      }
      tree_table.add(keys, Value(n.get_child(out).value(), output_type));
    }
  }

  inline Value eval(Sample &sample) const override {
    std::vector<Value> keys;
    Value tmp;
    bool missing_input = false;

    for (const auto &column : raw_columns) {
      tmp = sample[column].value;
      if (tmp.missing) missing_input = true;

      keys.push_back(tmp);
    }

    if (missing_input) {
      if (exist_missingreplacement) {
        return mapmissing_to;
      } else {
        return Value();
      }
    }

    try {
      return tree_table.get(keys);
    } catch (const std::out_of_range &e) {
      if (exist_defaultvalue)
        return defaultValue;
      else if (exist_missingreplacement)
        return mapmissing_to;
      else
        return Value();
      //            else throw NeverSawException("Input value " +
      //            ::mkstring<Value>(keys) + " never saw");
    }
  }
};

#endif
