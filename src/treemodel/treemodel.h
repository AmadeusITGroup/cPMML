
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_TREEMODEL_H
#define CPMML_TREEMODEL_H

#include <sstream>
#include <string>
#include <vector>

#include "core/datadictionary.h"
#include "core/internal_model.h"
#include "core/miningfield.h"
#include "core/miningfunction.h"
#include "core/miningschema.h"
#include "core/string_view.h"
#include "core/xmlnode.h"
#include "node.h"

/**
 * @class TreeModel
 *
 * Implementation of InternalModel representing a <a
 * href="http://dmg.org/pmml/v4-4/TreeModel.html">PMML TreeModel</a>.
 *
 * Through this class are represented Decision Tree models, both for
 * classification and regression.
 */
class TreeModel : public InternalModel {
 public:
  bool return_last_prediction = false;
  Node root_node;

  TreeModel() = default;

  TreeModel(const XmlNode &node, const DataDictionary &data_dictionary, const PredicateBuilder &predicate_builder,
            const std::shared_ptr<Indexer> &indexer)
      : InternalModel(node, data_dictionary, indexer),
        return_last_prediction(node.get_attribute("noTrueChildStrategy") == "returnLastPrediction"),
        root_node(Node(node.get_child("Node"), true, predicate_builder, target_field.datatype)){};

  TreeModel(const XmlNode &node, const DataDictionary &data_dictionary,
            const TransformationDictionary &transformationDictionary, const std::shared_ptr<Indexer> &indexer)
      : InternalModel(node, data_dictionary, transformationDictionary, indexer),
        return_last_prediction(node.get_attribute("noTrueChildStrategy") == "returnLastPrediction"),
        root_node(Node(node.get_child("Node"), true, PredicateBuilder(indexer), target_field.datatype)){};

  inline std::unique_ptr<InternalScore> score_raw(const Sample &sample) const override {
    return make_unique<TreeScore>(scoreR(sample, root_node));
  };

  inline std::string predict_raw(const Sample &sample) const override {
    return simple_scoreR(sample, root_node).to_string();
  };

  inline TreeScore scoreR(const Sample &sample, const Node &current_node) const {
#ifdef DEBUG
    static int depth = 0;
    //        depth++;
    std::cout << std::string(depth, '\t') << current_node.predicate << (current_node.leaf ? " !!! MATCH !!!" : "")
              << std::endl;
//        if(current_node.leaf)
//            depth--;
#endif

    TreeScore result;

    if (current_node.leaf) return current_node.score;

    for (const auto &child : current_node.children)
      if (child.match(sample)) {
#ifdef DEBUG
        depth++;
#endif
        result = scoreR(sample, child);

#ifdef DEBUG
        depth--;
//                if(result.score != "") depth--;
#endif
        if (result.is_score) return result;
      }

    if (return_last_prediction) return current_node.score;

    return TreeScore();
  }

  inline string_view simple_scoreR(const Sample &sample, const Node &current_node) const {
#ifdef DEBUG
    static int depth = 0;
    depth++;
    std::cout << std::string(depth, '\t') << current_node.predicate << (current_node.leaf ? " !!! MATCH !!!" : "")
              << std::endl;
    if (current_node.leaf) depth--;
#endif

    string_view result;

    if (current_node.leaf) return string_view(current_node.simple_score);

    for (const auto &child : current_node.children)
      if (child.match(sample)) {
        result = simple_scoreR(sample, child);
#ifdef DEBUG
        if (result != "") depth--;
#endif
        if (result != string_view()) return result;
      }

    if (return_last_prediction) return string_view(current_node.simple_score);

    return result;
  }
};

#endif
