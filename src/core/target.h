/*******************************************************************************

 * Copyright 2019 AMADEUS. All rights reserved.

 * Author: Paolo Iannino

 *******************************************************************************/

#ifndef CPMML_SRC_CORE_TARGET_H_
#define CPMML_SRC_CORE_TARGET_H_

#include <functional>
#include <string>
#include <unordered_map>

#include "internal_score.h"
#include "math/misc.h"
#include "miningfunction.h"
#include "miningschema.h"
#include "optype.h"
#include "transformationdictionary.h"
#include "xmlnode.h"

/**
 * @class Target
 *
 * Class representing <a href="http://dmg.org/pmml/v4-4/Targets.html">PMML
 * Target</a>.
 *
 * It defines transformations for the raw output of a model.
 */
class CastInteger {
 public:
  bool empty = true;
  std::function<double(const double)> cast_function;

  CastInteger() = default;
  explicit CastInteger(const std::string &cast_integer) {
    static std::unordered_map<std::string, std::function<double(const double)>> cast_integer_converter = {
        {"round", _round},
        {"ceiling", _ceil},
        {"floor", _floor},
    };

    auto result = cast_integer_converter.find(cast_integer);
    if (result != cast_integer_converter.cend()) {
      empty = false;
      cast_function = result->second;
    } else {
      empty = true;
      cast_function = _identity;
    }
  }

  constexpr double operator()(const double value) { return cast_function(value); }
};

class TargetValue {
 public:
  std::string value;
  bool has_display_value;
  std::string display_value;
  bool has_prior_probability;
  double prior_probability;
  double default_value;
  // TODO: Partition

  TargetValue()
      : has_display_value(false),
        has_prior_probability(false),
        prior_probability(double_min()),
        default_value(double_min()) {}

  TargetValue(const XmlNode &node)
      : value(node.get_attribute("value")),
        has_display_value(node.exists_attribute("displayValue")),
        display_value(node.get_attribute("displayValue")),
        has_prior_probability(node.exists_attribute("priorProbability")),
        prior_probability(node.get_double_attribute("priorProbability")),
        default_value(node.get_double_attribute("defaultValue")) {}

  static std::vector<TargetValue> to_targetvalues(std::vector<XmlNode> nodes) {
    std::vector<TargetValue> result;
    for (const auto &node : nodes) result.push_back(TargetValue(node));

    return result;
  }
};

class Target {
 public:
  MiningFunction mining_function;
  bool has_field_name = false;
  std::string field_name;
  CastInteger cast;
  bool has_min = false;
  double min = double_min();
  bool has_max = false;
  double max = double_min();
  bool has_rescale_constant = false;
  double rescale_constant = double_min();
  bool has_rescale_factor = false;
  double rescale_factor = double_min();
  OpType optype;
  std::vector<TargetValue> target_values;
  std::function<void(InternalScore &)> transform_score;

  Target() = default;

  explicit Target(const XmlNode &node, const MiningSchema &mining_schema,
                  const TransformationDictionary &transformation_dictionary, const MiningFunction &mining_fuction)
      : mining_function(mining_fuction),
        has_field_name(node.exists_attribute("field")),
        field_name(node.get_attribute("field")),
        cast(node.get_attribute("castInteger")),
        has_min(node.exists_attribute("min")),
        min(node.get_double_attribute("min")),
        has_max(node.exists_attribute("max")),
        max(node.get_double_attribute("max")),
        has_rescale_constant(node.exists_attribute("rescaleConstant")),
        rescale_constant(node.get_double_attribute("rescaleConstant")),
        has_rescale_factor(node.exists_attribute("rescaleFactor")),
        rescale_factor(node.get_double_attribute("rescaleFactor")),
        target_values(TargetValue::to_targetvalues(node.get_childs("TargetValue"))) {}

  inline void operator()(InternalScore &score) const {
    switch (mining_function.value) {
      case MiningFunction::MiningFunctionType::CLASSIFICATION:
        for (const auto &target_value : target_values) {
          if (score.score == target_value.value && target_value.has_display_value) {
            score.score = target_value.display_value;
            try {
              score.double_score = to_double(score.score);
            } catch (const cpmml::ParsingException &e) {
              score.double_score = double_min();
            }
          }

          if (score.probabilities.find(target_value.value) == score.probabilities.cend()) {
            if (target_value.has_prior_probability) {
              if (target_value.has_display_value) {
                score.probabilities[target_value.display_value] = target_value.prior_probability;
              } else {
                score.probabilities[target_value.value] = target_value.prior_probability;
              }
            } else {
              if (target_value.has_display_value) {
                score.probabilities[target_value.display_value] = score.probabilities[target_value.value];
                score.probabilities.erase(target_value.value);
              }
            }
          }
        }
        break;

      case MiningFunction::MiningFunctionType::REGRESSION:
        if (score.empty) {
          score.double_score = target_values[0].default_value;
          score.score = std::to_string(score.double_score);
        } else {
          if (has_min && score.double_score < min)
            score.double_score = min;
          else if (has_max && score.double_score > max)
            score.double_score = max;

          if (has_rescale_factor) score.double_score *= rescale_factor;
          if (has_rescale_constant) score.double_score += rescale_constant;
          if (!cast.empty) score.double_score = cast(score.double_score);

          // score string representation must be updated accordingly
          score.score = std::to_string(score.double_score);
        }
        break;
    }
  }

  inline std::string operator()(const std::string &predicted) const {
    double predicted_double = double_min();

    switch (mining_function.value) {
      case MiningFunction::MiningFunctionType::CLASSIFICATION:
        for (const auto &target_value : target_values) {
          if (predicted == target_value.value && target_value.has_display_value) return target_value.display_value;
        }
        break;

      case MiningFunction::MiningFunctionType::REGRESSION:
        predicted_double = to_double(predicted);
        if (predicted == "")
          return std::to_string(target_values[0].default_value);
        else {
          if (has_min && predicted_double < min)
            predicted_double = min;
          else if (has_max && predicted_double > max)
            predicted_double = max;

          if (has_rescale_factor) predicted_double *= rescale_factor;
          if (has_rescale_constant) predicted_double += rescale_constant;
          if (!cast.empty) predicted_double = cast(predicted_double);

          // score string representation must be updated accordingly
          return std::to_string(predicted_double);
        }
        break;
    }

    return predicted;
  }
};

#endif  // CPMML_SRC_CORE_TARGET_H_
