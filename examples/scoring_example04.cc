
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#include <iostream>
#include <vector>
#include <unordered_map>

#include "cPMML.h"

int main() {
  std::string linear_regression_filepath = "../benchmark/data/model/IrisMultinomReg.xml";
  std::string decision_tree_filepath = "../benchmark/data/model/IrisTree.xml";
  std::vector<cpmml::Model> models;
  std::vector<cpmml::Prediction> predictions;
  std::unordered_map<std::string, std::string> sample = {
      {"sepal_length", "6.6"},
      {"sepal_width", "2.9"},
      {"petal_length", "4.6"},
      {"petal_width", "1.3"}
  };

  models.push_back(cpmml::Model(linear_regression_filepath));
  models.push_back(cpmml::Model(decision_tree_filepath));
  
  for(const auto& model : models)
    predictions.push_back(model.score(sample));

  for(const auto& prediction : predictions)
    std::cout << prediction.as_string() << std::endl;

  return 0;
}
