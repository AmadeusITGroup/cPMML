
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_MININGFIELD_H
#define CPMML_MININGFIELD_H

#include <sstream>
#include <string>
#include <vector>

#include "fieldusagetype.h"
#include "invalidvaluetreatmentmethod.h"
#include "missingvaluetreatmentmethod.h"
#include "optype.h"
#include "outliertreatmentmethod.h"
#include "xmlnode.h"

/**
 * @class MiningField
 *
 * Class representing  <a href="http://dmg.org/pmml/v4-4/MiningSchema.html">PMML
 * MiningField</a>.
 *
 * It defines a field used by the model, along with its usage type and the
 * behaviour in case of missing/invalid values. See also: DataDictionary,
 * InvalidValueTreatmentMethod, MissingValueTreatmentMethod,
 * OutlierTreatmentMethos, FieldUsageType.
 */
class MiningField : public DataField {
 public:
  bool empty = true;
  FieldUsageType field_usage_type;
  OpType optype;
  double importance = double_min();
  bool hasOutlierTreatment = false;
  OutlierTreatmentMethod outliertreatmentmethod;
  bool hasLowValue = false;
  Value low_value;
  bool hasHighValue = false;
  Value high_value;
  bool hasMissingReplacement = false;
  Value missingvalue_replacement;
  MissingValueTreatmentMethod missingvalue_treatmentmethod;
  bool hasInvalidTreatment = false;
  InvalidValueTreatmentMethod invalidvalue_treatmentmethod;

  MiningField() = default;

  MiningField(const std::string &name, const DataType &datatype) : DataField(name, datatype), empty(false) {}

  MiningField(XmlNode node, const DataField &data_field)
      : DataField(data_field),
        empty(false),
        field_usage_type(node.get_attribute("usageType")),
        optype((node.get_attribute("optype"))),
        importance(node.get_double_attribute("importance")),
        hasOutlierTreatment(node.exists_attribute("outliers")),
        outliertreatmentmethod(OutlierTreatmentMethod(node.get_attribute("outliers"))),
        hasLowValue(node.exists_attribute("lowValue")),
        low_value(hasOutlierTreatment && hasLowValue ? Value(node.get_attribute("lowValue"), datatype) : Value()),
        hasHighValue(node.exists_attribute("highValue")),
        high_value(hasOutlierTreatment && hasHighValue ? Value(node.get_attribute("highValue"), datatype) : Value()),
        hasMissingReplacement(node.exists_attribute("missingValueReplacement")),
        missingvalue_replacement(hasMissingReplacement ? Value(node.get_attribute("missingValueReplacement"), datatype)
                                                       : Value()),  //, data_dictionary.datafields[name].datatype)),
        missingvalue_treatmentmethod(node.get_attribute("missingValueTreatmentMethod")),  // useless for scoring (just
                                                                                          // for info where the value
                                                                                          // comes from)
        hasInvalidTreatment(node.exists_attribute("invalidValueTreatmentMethod")),
        invalidvalue_treatmentmethod(
            hasInvalidTreatment ? InvalidValueTreatmentMethod(node.get_attribute("invalidValueTreatmentMethod"))
                                : InvalidValueTreatmentMethod()){};

  static std::unordered_map<std::string, MiningField> to_miningfields(std::vector<XmlNode> nodes,
                                                                      const DataDictionary &data_dictionary) {
    std::unordered_map<std::string, MiningField> result;
    for (auto it = nodes.cbegin(); it != nodes.cend(); it++) {
      if (data_dictionary.datafields.find(it->get_attribute("name")) == data_dictionary.datafields.cend())
        continue;  // this field is an output

      MiningField mining_field(*it, data_dictionary[it->get_attribute("name")]);
      result.insert(std::make_pair(mining_field.name, mining_field));
    }

    return result;
  }

  inline Value handle_missing() const { return hasMissingReplacement ? missingvalue_replacement : Value(); }

  inline bool is_outlier(const Value &value) const {
    // if it doesn't have low or high value it must be treated "asIs"
    // since other treatment methods imply low and high value. If treated "asIs"
    // then it's not considered an outlier

    return hasOutlierTreatment && hasLowValue && hasHighValue && (value < low_value || value > high_value);
  }

  inline Value handle_outlier(const Value &value) const {
    switch (outliertreatmentmethod.value) {  // in this two cases it must have low or high value
      case OutlierTreatmentMethod::OutlierTreatmentMethodValue::AS_MISSING_VALUES:
        return handle_missing();
      case OutlierTreatmentMethod::OutlierTreatmentMethodValue::AS_EXTREME_VALUES:
        if (value < low_value) return low_value;
        return high_value;
      default:
        return value;  // "asIs" treatment
    }
  }

  inline bool is_invalid(const Value &value) const { return !constraints(value); }

  inline Value handle_invalid(const Value &value) const {
    switch (invalidvalue_treatmentmethod.value) {
      case InvalidValueTreatmentMethod::InvalidValueTreatmentMethodValue::RETURN_INVALID:
        throw cpmml::InvalidValueException("Invalid value for field: " + name);
      case InvalidValueTreatmentMethod::InvalidValueTreatmentMethodValue::AS_MISSING:
        return handle_missing();
      default:
        return value;  // "asIs" treatment
    }
  }
};

#endif
