
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_DATADICTIONARY_H
#define CPMML_DATADICTIONARY_H

#include <iostream>
#include <sstream>
#include <string>

#include "datafield.h"
#include "datatype.h"
#include "indexer.h"
#include "optype.h"

/**
 * @class DataDictionary
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/DataDictionary.html">PMML DataDictionary</a>.
 *
 * It is a collection of DataFields. See DataField.
 */
class DataDictionary {
 public:
  long number_of_fields = 0;
  std::unordered_map<std::string, DataField> datafields;

  DataDictionary() = default;

  DataDictionary(const XmlNode &node, const std::shared_ptr<Indexer> &indexer)
      : number_of_fields(node.get_long_attribute("numberOfFields")),
        datafields(DataField::to_datafields(node.get_childs("DataField"), indexer)) {}

  inline const DataField &operator[](const std::string &feature_name) const { return datafields.at(feature_name); }

  inline const DataField &at(const std::string &feature_name) const { return datafields.at(feature_name); }
};

#endif
