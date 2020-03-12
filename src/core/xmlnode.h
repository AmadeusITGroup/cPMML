
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_XMLNODE_H
#define CPMML_XMLNODE_H

#include <limits>
#include <string>

#include "options.h"
#include "rapidxml-1.13/rapidxml.hpp"
#include "utils/utils.h"

// This class does not release resources yet -> see rapidxml

/**
 * @class XmlNode
 *
 * Non-owning wrapper of rapidxml::xml_node<> *, implementing some utility
 * functions.
 */
class XmlNode {
 public:
  XmlNode() = default;

  explicit XmlNode(rapidxml::xml_node<> *node) : node(node){};

  inline const std::string name() const { return node->name(); }

  inline const std::string value() const { return node->value(); }

  bool exists_attribute(const std::string &attribute_name) const {
    rapidxml::xml_attribute<> *attribute = node->first_attribute(attribute_name.c_str());

    return attribute != NULL;
  }

  const std::string get_attribute(const std::string &attribute_name) const {
    rapidxml::xml_attribute<> *attribute = node->first_attribute(attribute_name.c_str());

    if (attribute == NULL) return "null";

    return attribute->value();
  }

  double get_double_attribute(const std::string &attribute_name) const {
    double result;

    try {
      result = std::stod(get_attribute(attribute_name));
    } catch (const std::invalid_argument &exception) {
      result = std::numeric_limits<double>::min();
    }

    return result;
  }

  bool get_bool_attribute(const std::string &attribute_name) const {
    std::string tmp = get_attribute(attribute_name);

    if (to_lower(tmp) == "true" || tmp == "1") return 1;

    return 0;
  }

  long get_long_attribute(const std::string &attribute_name) const {
    long result;

    try {
      result = std::stol(get_attribute(attribute_name));
    } catch (const std::invalid_argument &exception) {
      result = std::numeric_limits<long>::max();
    }

    return result;
  }

  XmlNode get_child(const std::string &child_name) const { return XmlNode(node->first_node(child_name.c_str())); }

  XmlNode get_child() const { return XmlNode(node->first_node()); }

  bool exists_child(const std::string &child_name) const { return node->first_node(child_name.c_str()) != NULL; }

  std::vector<XmlNode> get_childs(const std::string &child_name) const {
    std::vector<XmlNode> result;

    //        if(node->first_node(child_name.c_str()) == NULL) throw
    //        cpmml::GenericException("No child found");

    for (rapidxml::xml_node<> *child = node->first_node(child_name.c_str()); child; child = child->next_sibling())
      if (child && child->name() == child_name) result.push_back(XmlNode(child));

    return result;
  }

  std::vector<XmlNode> get_childs() const {
    std::vector<XmlNode> result;

    //        if(node->first_node(child_name.c_str()) == NULL) throw
    //        cpmml::GenericException("No child found");

    for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling())
      result.push_back(XmlNode(child));

    return result;
  }

  XmlNode get_child_bypattern(const std::string &pattern) const {
    //        if(node->first_node(child_name.c_str()) == NULL) throw
    //        cpmml::GenericException("No child found");

    for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling()) {
      XmlNode child_node(child);

      if (to_lower(child_node.name()).find(to_lower(pattern)) != std::string::npos) {
        return child_node;
      }
    }

    return XmlNode();
  }

  XmlNode get_child_bylist(const std::unordered_set<std::string> &list) const {
    //        if(node->first_node(child_name.c_str()) == NULL) throw
    //        cpmml::GenericException("No child found");

    for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling()) {
      XmlNode child_node(child);

      if (list.find(child_node.name()) != list.cend()) {
        return child_node;
      }
    }

    return XmlNode();
  }

  bool exists_child_bylist(const std::unordered_set<std::string> &list) const {
    //        if(node->first_node(child_name.c_str()) == NULL) throw
    //        cpmml::GenericException("No child found");
    if (node->first_node() == nullptr) return false;

    for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling()) {
      XmlNode child_node(child);

      if (list.find(child_node.name()) != list.cend()) {
        return true;
      }
    }

    return false;
  }

  std::vector<XmlNode> get_childs_bypattern(const std::string &pattern) const {
    std::vector<XmlNode> result;

    //        if(node->first_node(child_name.c_str()) == NULL) throw
    //        cpmml::GenericException("No child found");

    for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling()) {
      XmlNode child_node(child);
      if (to_lower(child_node.name()).find(pattern) != std::string::npos) result.push_back(child_node);
    }

    return result;
  }

  std::vector<XmlNode> get_childs_byattribute(const std::string &child_name, const std::string &attribute_name,
                                              const std::string &attribute_value) const {
    std::vector<XmlNode> result;

    //        if(node->first_node(child_name.c_str()) == NULL) throw
    //        cpmml::GenericException("No child found");

    for (rapidxml::xml_node<> *child = node->first_node(child_name.c_str()); child; child = child->next_sibling())
      if (child != NULL && child->name() == child_name)
        if ((XmlNode(child)).get_attribute(attribute_name) == attribute_value) result.push_back(XmlNode(child));

    return result;
  }

  std::vector<XmlNode> get_childs_bylist(const std::unordered_set<std::string> &list) {
    std::vector<XmlNode> result;

    //        if(node->first_node(child_name.c_str()) == NULL) throw
    //        cpmml::GenericException("No child found");

    for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling()) {
      XmlNode child_node(child);

      if (list.find(child_node.name()) != list.cend()) {
        result.push_back(child_node);
      }
    }

    return result;
  }

  bool is_empty() { return node == NULL; }

 private:
  rapidxml::xml_node<> *node = NULL;
};

#endif
