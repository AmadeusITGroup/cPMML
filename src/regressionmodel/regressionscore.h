
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_REGRESSIONSCORE_H
#define CPMML_REGRESSIONSCORE_H

#include <vector>

#include "core/internal_score.h"
#include "utils/utils.h"

/**
 * @class RegressionScore
 *
 * Implementation of InternalScore for regression models.
 */
class RegressionScore : public InternalScore {
 public:
  RegressionScore(const std::string &simple_score, const double &simple_scored, const std::vector<std::string> &classes,
                  const std::vector<double> &scores)
      : InternalScore(simple_score, get_probabilities(classes, scores)) {}

  static std::unordered_map<std::string, double> get_probabilities(const std::vector<std::string> &classes,
                                                                   const std::vector<double> &scores) {
    std::unordered_map<std::string, double> probabilities;

    for (auto i = 0u; i < classes.size(); i++) probabilities[classes[i]] = scores[i];

    return probabilities;
  }
};

#endif
