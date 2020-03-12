
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_REGRESSIONTABLE_H
#define CPMML_REGRESSIONTABLE_H

#include <vector>
#include "categoricalpredictor.h"
#include "numericpredictor.h"
#include "predictorterm.h"

/**
 * @class RegressionTable
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Regression.html#xsdElement_RegressionTable">PMML
 * Regression Table</a>.
 *
 * It is the class wrapping all predictors for the RegressionModel:
 * NumericPredictor, CategoricalPredictor and PredictorTerm. For predicting a
 * numerical value RegressionModel contains just one RegressionTable. For
 * predicting a categorical value, it contains one RegressionTable for each
 * category involved.
 */
class RegressionTable {
 public:
  double intercept = 0;
  std::string target_category;
  std::vector<NumericPredictor> numeric_predictors;
  std::vector<CategoricalPredictor> categorical_predictors;
  std::vector<PredictorTerm> predictor_terms;

  RegressionTable() = default;

  RegressionTable(const XmlNode &node, const std::shared_ptr<Indexer> &indexer)
      : intercept(node.get_double_attribute("intercept")),
        target_category(node.get_attribute("targetCategory")),
        numeric_predictors(NumericPredictor::to_numericpredictors(node.get_childs("NumericPredictor"), indexer)),
        categorical_predictors(CategoricalPredictor::to_categoricalpredictors(node, indexer)),
        predictor_terms(PredictorTerm::to_predictorterms(node.get_childs("PredictorTerm"), indexer)) {}

  inline double score(const Sample &sample) const {
    double partial = 0;
    for (const auto &numeric_predictor : numeric_predictors) partial += numeric_predictor.get_term(sample);
    for (const auto &categorical_predictor : categorical_predictors) partial += categorical_predictor.get_term(sample);
    for (const auto &predictor_term : predictor_terms) partial += predictor_term.get_term(sample);

    return intercept + partial;
  }

  static std::vector<RegressionTable> to_regressiontables(const std::vector<XmlNode> &nodes,
                                                          const std::shared_ptr<Indexer> &indexer) {
    std::vector<RegressionTable> res;
    for (const auto &node : nodes) res.push_back(RegressionTable(node, indexer));

    return res;
  }
};

#endif
