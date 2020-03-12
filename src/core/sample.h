
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_SAMPLE_H
#define CPMML_SAMPLE_H

#include <map>
#include <memory>
#include <string>

#include "value.h"

/**
 * @class Feature
 *
 * Class used as an internal representation of the user provided feature.
 *
 * It allows to signal when the corresponding value is missing.
 */
class Feature {
 public:
#ifdef DEBUG
  std::string name = "null";
#endif
  Value value = Value();

  Feature() = default;

  Feature(const std::string &name, const Value &value)
#ifdef DEBUG
      : name(name),
        value(value)
#endif
  {
  }

#ifdef DEBUG
  const std::string to_string() const { return "{\"" + name + "\": " + value.to_string() + "}"; };
#endif

  inline const Value &cvalue() const {
    if (value.missing) throw cpmml::MissingValueException("missing value");

    return value;
  }
};

/**
 * @class Sample
 *
 * Class used as an internal representation of the user provided sample.
 *
 * It stores the values for each feature used by the model. The features are
 * indexed thanks to Indexer class. This allows to reach constant access time.
 */
class Sample {
 public:
  Sample() {}

  explicit Sample(const size_t &size) : features(size) {}

  inline Feature &operator[](const size_t &feature_index) { return features[feature_index]; }

  inline const Feature &operator[](const size_t &feature_index) const { return features[feature_index]; }

  inline void change_value(const size_t &feature_index, const Value &value) { features[feature_index].value = value; }

  inline void change_value_if_missing(const size_t &feature_index, const Value &value) {
    if (features[feature_index].value.missing) {
      features[feature_index].value = value;
      features[feature_index].value.missing = false;
    }
  }

  std::vector<Feature> features;
};

#endif
