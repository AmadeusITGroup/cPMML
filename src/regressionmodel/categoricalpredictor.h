
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_CATEGORICALPREDICTOR_H
#define CPMML_CATEGORICALPREDICTOR_H

#include "core/datatype.h"
#include "core/sample.h"
#include "core/value.h"
#include "core/xmlnode.h"

/**
 * @class CategoricalPredictor
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Regression.html#xsdElement_CategoricalPredictor">PMML
 * CategoricalPredictor</a>.
 *
 * It contains the parameters (coefficients and exponents) needed to compute the
 * regression score for a single category among the ones involved in the
 * prediction of a categorical variable.
 */
class CategoricalPredictor {
 public:
  std::string name;
  size_t index = std::numeric_limits<size_t>::max();
  DataType datatype;
  std::unordered_map<Value, double, Value::ValueHash> coefficients;

  CategoricalPredictor() = default;

  CategoricalPredictor(const XmlNode &node,  // Regression table node
                       const std::string &name, const size_t &index, const DataType &datatype)
      : name(name),
        index(index),
        datatype(datatype),
        coefficients(
            get_coefficients(node.get_childs_byattribute("CategoricalPredictor", "name", name), index, datatype)) {}

  inline double get_term(const Sample &sample) const {
    Value value = sample[index].value;

    if (value.missing || coefficients.find(value) == coefficients.cend()) return 0;

    return coefficients.at(value);
  }

  static std::unordered_map<Value, double, Value::ValueHash> get_coefficients(const std::vector<XmlNode> &nodes,
                                                                              const size_t &index,
                                                                              const DataType &datatype) {
    std::unordered_map<Value, double, Value::ValueHash> result;
    for (const auto &node : nodes)
      result[Value(node.get_attribute("value"), datatype)] = ::to_double(node.get_attribute("coefficient"));

    return result;
  }

  static std::vector<CategoricalPredictor> to_categoricalpredictors(
      const XmlNode &regressiontable_node,  // regression table node needed by
                                            // each categorical predictor
      const std::shared_ptr<Indexer> &indexer) {
    std::vector<CategoricalPredictor> result;

    std::string field_name;
    size_t field_index;
    DataType field_datatype;
    std::unordered_set<std::string> seen;
    for (const auto &node : regressiontable_node.get_childs("CategoricalPredictor")) {
      field_name = node.get_attribute("name");
      if (seen.find(field_name) == seen.cend()) {
        field_index = indexer->get_index(field_name);
        field_datatype = indexer->get_type(field_name);
        result.push_back(CategoricalPredictor(regressiontable_node, field_name, field_index, field_datatype));
        seen.insert(field_name);
      }
    }

    return result;
  }
};

#endif
