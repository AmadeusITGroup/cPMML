
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#include <iostream>
#include <unordered_map>

#include "cPMML.h"
#include "utils/csvreader.h"
#include "utils/utils.h"

// errors are admitted in regression up to 0.1% of the actual prediction
double regression_relative_error_tolerance = 0.001;

inline double regression_error(const cpmml::Prediction &prediction,
                               const std::unordered_map<std::string, std::string> &sample) {
  if (prediction.as_double() != double_min())  // check double score available
    return std::abs(prediction.as_double() - to_double(sample.at("prediction"))) / to_double(sample.at("prediction"));

  return 0.0;
}

int main(int argc, char **argv) {
  cpmml::Model model(argv[1], true);
  CSVReader reader(argv[2]);
  std::unordered_map<std::string, std::string> sample;

  while ((sample = reader.read()).size() > 0) {
    cpmml::Prediction prediction = model.score(sample);
    if ((prediction.as_string() != sample["prediction"] or prediction.as_string() != model.predict(sample)) and
        regression_error(prediction, sample) >= regression_relative_error_tolerance) {
      std::cerr << "predicted: " << prediction.as_string();
      std::cerr << " true: " << sample["prediction"] << " sample: " << to_string(sample) << std::endl;
      return -1;
    }
  }

  return 0;
}
