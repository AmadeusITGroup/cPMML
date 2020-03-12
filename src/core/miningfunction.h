
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_MININGFUNCTION_H
#define CPMML_MININGFUNCTION_H

#include <map>
#include <string>

/**
 * @class MiningFunction
 *
 * Class representing  <a
 * href="http://dmg.org/pmml/v4-4/GeneralStructure.html#xsdType_MINING-FUNCTION">PMML
 * MiningFunction</a>.
 *
 * It defines which is the function pursued by the model.
 *
 * For instance:
 * - Classification
 * - Regression
 * - Clustering
 * - etc.
 */
class MiningFunction {
 public:
  enum class MiningFunctionType {
    //    ASSOCIATION_RULES,
    //    SEQUENCES,
    CLASSIFICATION,
    REGRESSION,
    //    CLUSTERING,
    //    TIMESERIES,
    //    MIXED,
    //    UNDEFINED
  };

  MiningFunctionType value;

  MiningFunction() = default;

  explicit MiningFunction(const std::string &mining_function) : value(from_string(mining_function)) {}

  static MiningFunctionType from_string(const std::string &mining_function) {
    const static std::map<std::string, MiningFunctionType> mining_function_converter = {
        //        {"associationRules", MiningFunctionType::ASSOCIATION_RULES},
        //        {"sequences", MiningFunctionType::SEQUENCES},
        {"classification", MiningFunctionType::CLASSIFICATION},
        {"regression", MiningFunctionType::REGRESSION},
        //        {"clustering", MiningFunctionType::CLUSTERING},
        //        {"timeSeries", MiningFunctionType::TIMESERIES},
        //        {"mixed", MiningFunctionType::MIXED}
    };

    try {
      return mining_function_converter.at(to_lower(mining_function));
    } catch (const std::out_of_range &exception) {
      throw cpmml::ParsingException("unsupported mining function: " + mining_function);
    }
  }

  std::string to_string() const {
    switch (value) {
        //      case MiningFunctionType::ASSOCIATION_RULES:
        //        return "ASSOCIATION_RULES";
        //      case MiningFunctionType::SEQUENCES:
        //        return "SEQUENCES";
      case MiningFunctionType::CLASSIFICATION:
        return "CLASSIFICATION";
      case MiningFunctionType::REGRESSION:
        return "REGRESSION";
        //      case MiningFunctionType::CLUSTERING:
        //        return "CLUSTERING";
        //      case MiningFunctionType::TIMESERIES:
        //        return "TIMESERIES";
        //      case MiningFunctionType::MIXED:
        //        return "MIXED";
        //      case MiningFunctionType::UNDEFINED:
        //        return "UNDEFINED";
      default:
        return "UNDEFINED";
    }
  }
};

#endif
