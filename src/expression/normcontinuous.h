
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_NORMCONTINUOUS_H
#define CPMML_NORMCONTINUOUS_H

#include <cstdlib>
#include <utility>

#include "expression.h"

/**
 * @class NormContinuous
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Transformations.html#xsdElement_NormContinuous">PMML
 * NormContinuous</a>.
 *
 * It is used to normalize input fields through a piecewise linear
 * interpolation.
 */
class NormContinuous : public Expression {
 public:
  bool exist_missingreplacement = false;
  Value mapmissing_to;
  std::string field_name;
  size_t index = std::numeric_limits<size_t>::max();
  OutlierTreatmentMethod outliertreatment_method;
  std::vector<Value> orig;
  std::vector<Value> norm;

  NormContinuous() = default;

  NormContinuous(const XmlNode &node, const unsigned int &output_index, const DataType &output_type,
                 const std::shared_ptr<Indexer> &indexer)
      : Expression(output_index, output_type, indexer),
        exist_missingreplacement(node.exists_attribute("mapMissingTo")),
        mapmissing_to(exist_missingreplacement ? Value(node.get_attribute("mapMissingTo"), output_type) : Value()),
        field_name(node.get_attribute("field")),
        index(indexer->get_or_set(field_name)),
        outliertreatment_method(node.get_attribute("outliers")),
        orig(get_values("orig", node,
                        output_type)),  // TYPE IMPLICIT: DOUBLE, we know input type must be
                                        // equal to output type in this case
        norm(get_values("norm", node,
                        output_type))  // TYPE IMPLICIT: DOUBLE, we know input type must be
                                       // equal to output type in this case
  {
    inputs.insert(field_name);
  }

  static std::vector<Value> get_values(const std::string &linearnorm_type, const XmlNode &node,
                                       const DataType &dataType) {
    std::vector<Value> result;

    for (const auto &linear_norm : node.get_childs_bypattern("LinearNorm"))
      result.push_back(Value(linear_norm.get_attribute("orig"), dataType));

    return result;
  }

  inline unsigned int get_interval(
      const Value &value) const {  // if value == orig then the closest interval will be right and
                                   // the interpolation also (look at formula and ifs to see it)
    unsigned int i, min_i = std::numeric_limits<unsigned int>::min();
    Value min(double_min(), DataType::DataTypeValue::DOUBLE), a, b;
    for (i = 0; i < (orig.size() - 1); i++) {
      if (value > orig[i] && value < orig[i + 1]) {
        return i;  // correct interval
      } else {
        a = value.diff(orig[i]);
        b = value.diff(orig[i + 1]);
        if (a < min || b < min) {
          min_i = i;
          if (a < min) min = a;
          if (b < min) min = b;
        }
      }
    }

    return min_i;  // nearest interval (see outlier below)
  }

  inline Value interpolate(const Value &value) const {
    unsigned int i = get_interval(value);
    return norm[i] + (value - orig[i]) / (orig[i + 1] - orig[i]) *
                         (norm[i + 1] - norm[i]);  // that's why input and output type must be equal
  }

  inline Value eval(Sample &sample) const override {
    Value input = sample[index].value;

    if (input < orig[0] && input > orig[1]) {
      switch (outliertreatment_method.value) {
        case OutlierTreatmentMethod::OutlierTreatmentMethodValue::AS_IS:
          return interpolate(input);
        case OutlierTreatmentMethod::OutlierTreatmentMethodValue::AS_MISSING_VALUES:
          return mapmissing_to;
        case OutlierTreatmentMethod::OutlierTreatmentMethodValue::AS_EXTREME_VALUES:
          return norm.back();
      }
    }

    return interpolate(input);  // no outlier
  }
};

#endif
