
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_DATAFIELD_H
#define CPMML_DATAFIELD_H

#include <sstream>
#include <string>

#include "closure.h"
#include "datatype.h"
#include "indexer.h"
#include "intervalbuilder.h"
#include "optype.h"
#include "predicate.h"
#include "property.h"
#include "value.h"
#include "xmlnode.h"

/**
 * @class DataField
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/DataDictionary.html">PMML DataField</a>.
 *
 * It defines a feature available to the model, along with the values it can
 * assume. The constraints on the admissible values are enforced through the
 * class Predicate.
 */
class DataField {
 public:
  size_t n_values = 1;
  std::string name;
  DataType datatype;
  size_t index = std::numeric_limits<size_t>::max();
  OpType optype;
  Value missing_replacement;
  Predicate constraints;

  DataField() = default;

  DataField(const std::string &name, const DataType &datatype)
      : n_values(1), name(name), datatype(datatype), index(std::numeric_limits<size_t>::max()) {}

  DataField(const XmlNode &node, const std::shared_ptr<Indexer> &indexer)
      : name(node.get_attribute("name")),
        datatype(node.get_attribute("dataType")),
        index(indexer->get_or_set(name, datatype).first),
        optype(node.get_attribute("optype")) {
    auto values = node.get_childs("Value");
    std::vector<Predicate> tmp_contraints;
    std::set<Value> allowed_values;
    std::set<Value> forbidden_values;
    for (const auto &value : values) {
      auto property = Property(value.get_attribute("property"));
      switch (property.value) {
        case Property::PropertyValue::VALID:
          allowed_values.emplace(Value(value.get_attribute("value"), datatype));
          break;
        case Property::PropertyValue::MISSING:
          missing_replacement = Value(value.get_attribute("value"), datatype);
          break;
        case Property::PropertyValue::INVALID:
          forbidden_values.emplace(Value(value.get_attribute("value"), datatype));
          break;
      }
    }
    if (allowed_values.size() > 0) tmp_contraints.push_back(Predicate(index, "isIn", allowed_values));
    if (forbidden_values.size() > 0) tmp_contraints.push_back(Predicate(index, "isNotIn", forbidden_values));

    auto intervals = node.get_childs("Interval");
    for (const auto &interval : intervals) tmp_contraints.push_back(IntervalBuilder::build(interval, index, datatype));

    if (tmp_contraints.size() > 0) constraints = Predicate(tmp_contraints, "AND");

    n_values = allowed_values.size() > 0 ? allowed_values.size() : 1;
  };

  inline bool validate(const Sample &sample) const { return constraints.is_empty ? true : constraints(sample); }
  inline Value createValue(const std::string &value) const { return Value(value, datatype); }

  static std::unordered_map<std::string, DataField> to_datafields(const std::vector<XmlNode> &nodes,
                                                                  const std::shared_ptr<Indexer> &indexer) {
    std::unordered_map<std::string, DataField> result;
    for (const auto &node : nodes) result[node.get_attribute("name")] = DataField(node, indexer);

    return result;
  }
};

#endif
