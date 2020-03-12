
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_HEADER_H
#define CPMML_HEADER_H

#include <sstream>
#include <string>
#include "xmlnode.h"

/**
 * @class Header
 *
 * Class representing  <a href="http://dmg.org/pmml/v4-4/Header.html">PMML
 * Header</a>.
 *
 * It contains some metadata for the model.
 */
class Header {
 public:
  std::string copyright;
  std::string description;
  std::string model_version;

  Header(){};

  explicit Header(const XmlNode &node)
      : copyright(node.get_attribute("copyright")),
        description(node.get_attribute("description")),
        model_version(node.get_attribute("modelVersion")){};
};

#endif
