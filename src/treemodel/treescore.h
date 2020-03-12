
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_TREESCORE_H
#define CPMML_TREESCORE_H

#include "core/datatype.h"
#include "core/internal_score.h"
#include "scoredistribution.h"

/**
 * @class TreeScore
 *
 * Implementation of InternalScore for TreeModel objects.
 */
class TreeScore : public InternalScore {
 public:
  bool is_score = false;
  DataType target_type;

  TreeScore() = default;

  TreeScore(const std::string &simple_score, const DataType &target_type,
            const std::vector<XmlNode> &score_distribution_nodes)
      : InternalScore(simple_score, get_probabilities(ScoreDistribution::to_score_distributions(
                                        score_distribution_nodes, target_type))),
        is_score(true),
        target_type(target_type) {}

  static std::unordered_map<std::string, double> get_probabilities(
      const std::vector<ScoreDistribution> &score_distributions) {
    std::unordered_map<std::string, double> probabilities;

    double total = 0;
    for (const auto &score_distribution : score_distributions) total += score_distribution.record_count;

    for (const auto &score_distribution : score_distributions)
      probabilities[score_distribution.value_string] = score_distribution.record_count / total;

    return probabilities;
  }

  static std::unordered_map<std::string, double> get_totals(const std::vector<ScoreDistribution> &score_distributions) {
    std::unordered_map<std::string, double> totals;

    for (const auto &score_distribution : score_distributions)
      totals[score_distribution.value_string] = score_distribution.record_count;

    return totals;
  }
};

#endif
