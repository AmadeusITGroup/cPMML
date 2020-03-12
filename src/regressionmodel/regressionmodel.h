
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_REGRESSIONMODEL_H
#define CPMML_REGRESSIONMODEL_H

#include <algorithm>

#include "core/datadictionary.h"
#include "core/internal_model.h"
#include "core/miningfunction.h"
#include "core/transformationdictionary.h"
#include "core/xmlnode.h"
#include "normalizationmethodbuilder.h"
#include "normalizationmethodtype.h"
#include "regressionscore.h"
#include "regressiontable.h"
#include "treemodel/treescore.h"

/**
 * @class RegressionModel
 *
 * Implementation of InternalModel representing a <a
 * href="http://dmg.org/pmml/v4-4/Regression.html">PMML RegressionModel</a>.
 *
 * Through this class are represented models such as Linear Regression Model,
 * Logistic Regression Model, etc.
 */
class RegressionModel : public InternalModel {
 public:
  NormalizationMethodType normalization_methodtype;
  std::function<double(const double &)> regression_normalization;
  std::function<std::vector<double>(const std::vector<double> &)> classification_normalization;
  std::vector<RegressionTable> regression_tables;
  std::vector<std::string> classes;

  RegressionModel() = default;

  RegressionModel(const XmlNode &node, const DataDictionary &data_dictionary, const std::shared_ptr<Indexer> &indexer)
      : InternalModel(node, data_dictionary, indexer),
        normalization_methodtype(node.get_attribute("normalizationMethod")),
        regression_normalization(SingleNormalizationMethodBuilder::build(normalization_methodtype)),
        classification_normalization(MultiNormalizationMethodBuilder::build(normalization_methodtype)),
        regression_tables(RegressionTable::to_regressiontables(node.get_childs("RegressionTable"), indexer)) {
    if (mining_function.value == MiningFunction::MiningFunctionType::CLASSIFICATION)
      for (const auto &regression_table : regression_tables) classes.push_back(regression_table.target_category);
    else
      classes.push_back(mining_schema.target.name);
  }

  RegressionModel(const XmlNode &node, const DataDictionary &data_dictionary,
                  const TransformationDictionary &transformationDictionary, const std::shared_ptr<Indexer> &indexer)
      : InternalModel(node, data_dictionary, transformationDictionary, indexer),
        normalization_methodtype(node.get_attribute("normalizationMethod")),
        regression_normalization(SingleNormalizationMethodBuilder::build(normalization_methodtype)),
        classification_normalization(MultiNormalizationMethodBuilder::build(normalization_methodtype)),
        regression_tables(RegressionTable::to_regressiontables(node.get_childs("RegressionTable"), indexer)) {
    if (mining_function.value == MiningFunction::MiningFunctionType::CLASSIFICATION)
      for (const auto &regression_table : regression_tables) classes.push_back(regression_table.target_category);
    else
      classes.push_back(mining_schema.target.name);
  }

  inline std::unique_ptr<InternalScore> score_raw(const Sample &sample) const override {
    std::vector<double> scores;
    double regressed_value;
    switch (mining_function.value) {
      case MiningFunction::MiningFunctionType::REGRESSION:
        scores.push_back(regression_normalization(regression_tables[0].score(sample)));
        regressed_value = scores[0];
        return make_unique<RegressionScore>(std::to_string(regressed_value), regressed_value, classes, scores);
      case MiningFunction::MiningFunctionType::CLASSIFICATION:
        scores = classification_normalization(get_scores(sample));
        regressed_value = *std::max_element(scores.begin(), scores.end());
        return make_unique<RegressionScore>(get_class(scores), regressed_value, classes, scores);
    }

    throw cpmml::ParsingException(mining_function.to_string() + "not available in RegressionModel");
  }

  inline std::string predict_raw(const Sample &sample) const override {
    switch (mining_function.value) {
      case MiningFunction::MiningFunctionType::REGRESSION:
        return std::to_string(regression_normalization(regression_tables[0].score(sample)));
      case MiningFunction::MiningFunctionType::CLASSIFICATION:
        return get_class(classification_normalization(get_scores(sample)));
    }

    throw cpmml::ParsingException(mining_function.to_string() + "not available in RegressionModel");
  }

  inline std::vector<double> get_scores(const Sample &sample) const {
    std::vector<double> scores;

    for (const auto &regressionTable : regression_tables) scores.push_back(regressionTable.score(sample));

    return scores;
  }

  inline std::string get_class(const std::vector<double> &scores) const {
    double max = -double_min();
    size_t _class = 0;

    for (auto i = 0u; i < regression_tables.size(); i++)
      if (scores[i] > max) {
        max = scores[i];
        _class = i;
      }

    return regression_tables[_class].target_category;
  }
};

#endif
