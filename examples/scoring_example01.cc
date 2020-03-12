
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#include <iostream>
#include <unordered_map>

#include "cPMML.h"

int main() {
  std::string model_filepath = "../benchmark/data/model/single_audit_dectree.xml";

  cpmml::Model model(model_filepath);
  std::unordered_map<std::string, std::string> sample = {
      {"TARGET_Adjusted", "1"},
      {"IGNORE_Accounts", "UnitedStates"},
      {"Hours", "50"},
      {"Deductions", "0"},
      {"RISK_Adjustment", "22418"},
      {"Gender", "Male"},
      {"ID", "1047698"},
      {"Income", "43391.17"},
      {"Age", "43"},
      {"Education", "Bachelor"},
      {"Marital", "Married"},
      {"Employment", "Private"},
      {"Occupation", "Executive"}
  };

  std::cout << "score: " << model.predict(sample) << std::endl;

  return 0;
}
