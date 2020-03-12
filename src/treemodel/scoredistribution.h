
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_SCOREDISTRIBUTION_H
#define CPMML_SCOREDISTRIBUTION_H

#include "core/value.h"
#include "core/xmlnode.h"

/**
 * @class ScoreDistribution
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/TreeModel.html#xsdElement_ScoreDistribution">PMML
 * ScoreDistribution</a>.
 *
 * It contains additional information for the score associated to a node. For
 * instance:
 * - Probability distribution.
 * - Confidence.
 */
class ScoreDistribution {
 public:
  Value value;
  std::string value_string;
  double record_count = double_min();
  bool has_confidence = false;
  double confidence = double_min();
  std::string confidence_string;
  bool has_probability = false;
  double probability = double_min();
  std::string probability_string;

  ScoreDistribution() = default;

  ScoreDistribution(XmlNode node, DataType dataType)
      : value(Value(node.get_attribute("value"), dataType)),
        value_string(node.get_attribute("value")),
        record_count(node.get_double_attribute("recordCount")),
        has_confidence(node.exists_attribute("confidence")),
        confidence(node.get_double_attribute("confidence")),
        confidence_string(node.get_attribute("confidence")),
        has_probability(node.exists_attribute("probability")),
        probability(node.get_double_attribute("probability")),
        probability_string(node.get_attribute("probability")) {}

  ScoreDistribution(XmlNode node, DataType dataType, double total_record_count)
      : value(Value(node.get_attribute("value"), dataType)),
        record_count(node.get_double_attribute("recordCount")),
        has_confidence(node.exists_attribute("confidence")),
        confidence(total_record_count > 0 ? record_count / total_record_count
                                          : 1),  // confidence should be computed with laplace correction,
                                                 // since no coefficient is prenset in PMML
        // when, as in this case, it is computed from record count, it is equal
        // to probability
        confidence_string(::to_string<double>(confidence)),
        has_probability(node.exists_attribute("probability")),
        probability(confidence),
        probability_string(confidence_string) {}

  static std::vector<ScoreDistribution> to_score_distributions(const std::vector<XmlNode> &score_distribution_nodes,
                                                               const DataType &target_type) {
    std::vector<ScoreDistribution> result;  // initialization just for perf improvement

    for (const auto &node : score_distribution_nodes) result.push_back(ScoreDistribution(node, target_type));

    return result;
  }
};

#endif
