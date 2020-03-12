
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_TRANSFORMATIONDICTIONARY_H
#define CPMML_TRANSFORMATIONDICTIONARY_H

#include "derivedfield.h"
#include "indexer.h"
#include "xmlnode.h"

/**
 * @class TransformationDictionary
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html">PMML
 * TransformationDictionary</a>.
 *
 * It is a collection of DerivedFields. See also DerivedField.
 */
class TransformationDictionary {
 public:
  bool empty = true;
  std::unordered_map<std::string, DerivedField> raw_derivedfields;
  std::vector<DerivedField> derivedfields;

  TransformationDictionary() = default;

  TransformationDictionary(const XmlNode &node, const std::shared_ptr<Indexer> &indexer)
      : empty(false),
        raw_derivedfields(DerivedField::to_derivedfields(node.get_childs("DerivedField"), indexer)),
        derivedfields(to_values<std::string, DerivedField>(raw_derivedfields)) {}

  inline bool contains(const std::string &field_name) const {
    return raw_derivedfields.find(field_name) != raw_derivedfields.cend();
  }

  inline const DerivedField &operator[](const std::string &feature_name) const {
    return raw_derivedfields.at(feature_name);
  }

  inline const DerivedField &operator[](const size_t &feature_index) const { return derivedfields[feature_index]; }

  inline void add_derived_field(const DerivedField &derived_field) {
    raw_derivedfields[derived_field.name] = derived_field;
    derivedfields.push_back(derived_field);
  }
};

#endif
