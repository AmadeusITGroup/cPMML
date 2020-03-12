
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_FIELDREF_H
#define CPMML_FIELDREF_H

#include <string>
#include "core/value.h"
#include "expression.h"

/**
 * @class FieldRef
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html#xsdElement_FieldRef">PMML
 * FieldRef</a>.
 *
 * It is used to simply reference (and rename) another DataField or
 * DerivedField.
 */
class FieldRef : public Expression {
 public:
  bool exist_missingreplacement = false;
  std::string field_name;
  size_t index = std::numeric_limits<size_t>::max();
  Value mapmissing_to;

  FieldRef() = default;

  FieldRef(const std::string &field_name, const std::shared_ptr<Indexer> &indexer, const size_t &output_index,
           const DataType &output_type)
      : Expression(output_index, output_type, indexer),
        exist_missingreplacement(false),
        field_name(field_name),
        index(indexer->get_or_set(field_name)) {}

  FieldRef(const XmlNode &node, const size_t &output_index, const DataType &output_type,
           const std::shared_ptr<Indexer> &indexer)
      : Expression(output_index, output_type, indexer),
        exist_missingreplacement(node.exists_attribute("mapMissingTo")),
        field_name(node.get_attribute("field")),
        index(indexer->get_or_set(field_name)),  // datatype is not needed here
        mapmissing_to(exist_missingreplacement ? Value(node.get_attribute("mapMissingTo"), output_type) : Value()) {
    inputs.insert(field_name);
  }

  inline Value eval(Sample &sample) const override {
    Value value = sample[index].value;
    return value.missing ? mapmissing_to : value;
  }
};

/**
 * @class SimpleFieldRef
 *
 * Class used to simply reference (and rename) another DataField or
 * DerivedField. It differs from FieldRef since it has local scope rather than
 * global scope.
 */
class SimpleFieldRef {
  // As fieldref but the derived field is supposed to have local visibility,
  // so it's not implementing Expression

 public:
  bool exist_missingreplacement;
  std::string field_name;
  size_t index;
  Value mapmissing_to;

  SimpleFieldRef() : exist_missingreplacement(false), index(std::numeric_limits<size_t>::max()) {}

  SimpleFieldRef(const XmlNode &node, const std::shared_ptr<Indexer> &indexer)
      : exist_missingreplacement(node.exists_attribute("mapMissingTo")),
        field_name(node.get_attribute("field")),
        index(indexer->get_index(field_name)),
        mapmissing_to(exist_missingreplacement
                          ? Value(node.get_attribute("mapMissingTo"), indexer->get_type(field_name))
                          : Value()) {}

  inline Value value(const Sample &sample) const {
    Value value = sample[index].value;
    return value.missing ? mapmissing_to : value;
  }

  static std::vector<SimpleFieldRef> to_simplefields(const std::vector<XmlNode> &nodes,
                                                     const std::shared_ptr<Indexer> &indexer) {
    std::vector<SimpleFieldRef> result;
    for (const auto &node : nodes) result.push_back(SimpleFieldRef(node, indexer));

    return result;
  }
};

#endif
