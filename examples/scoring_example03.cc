
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#include <iostream>
#include <unordered_map>

#include "cPMML.h"

int main() {
  std::string model_filepath = "../test/data/model/IrisLinearReg.zip";

  cpmml::Model model(model_filepath, true);
  std::unordered_map<std::string, std::string> sample = {
      {"sepal_width", "2.9"},
      {"petal_length", "4.6"},
      {"petal_width", "1.3"},
      {"class", "Iris-versicolor"}
  };

  for (const auto &probability : model.score(sample).distribution())
    std::cout << probability.first << ": " << probability.second << std::endl;

  return 0;
}
