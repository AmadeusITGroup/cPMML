
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_NODE_H
#define CPMML_NODE_H

#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "core/fastpredicate.h"
#include "core/predicatebuilder.h"
#include "core/xmlnode.h"
#include "scoredistribution.h"
#include "treescore.h"

/**
 * @class Node
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/TreeModel.html#xsdElement_Node">PMML Node</a>.
 *
 * It is a node of the decision tree, containing a Predicate and a TreeScore.
 * The score represents the prediction associated to a sample matching the
 * predicate.
 */
class Node {
 public:
  //    std::string id;
  std::string simple_score;
  double record_count = double_min();
  //    std::string default_child;
  std::vector<Node> children;
  std::function<bool(const Sample &)> predicate;
  bool root = false;
  bool leaf = false;
  TreeScore score;

  Node() = default;

  Node(const XmlNode &node, bool root, const PredicateBuilder &predicate_builder,
       const DataType &target_datatype)
      :  //        id(node.get_attribute("id")),
        simple_score(node.exists_attribute("score") ? node.get_attribute("score") : ""),
        record_count(node.get_double_attribute("recordCount")),
        //        default_child(node.get_attribute("defaultChild")),
        children(to_nodes(node.get_childs("Node"), predicate_builder, target_datatype)),
        predicate(to_function(predicate_builder.build(node.get_child_bypattern("Predicate")))),
        root(root),
        leaf(children.size() == 0),
        score(simple_score, target_datatype, node.get_childs("ScoreDistribution")){};

  constexpr bool match(const Sample &sample) const { return predicate(sample); }

  static std::vector<Node> to_nodes(const std::vector<XmlNode> &nodes, const PredicateBuilder &predicateBuilder,
                                    const DataType &target_datatype) {
    std::vector<Node> result;
    for (auto node : nodes) result.push_back(Node(node, false, predicateBuilder, target_datatype));

    return result;
  }
};

#endif
