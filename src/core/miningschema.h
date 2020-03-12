
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_MININGSCHEMA_H
#define CPMML_MININGSCHEMA_H

#include <unordered_map>

#include "datafield.h"
#include "miningfield.h"

/**
 * @class MiningSchema
 *
 * Class Representing  <a href="http://dmg.org/pmml/v4-4/MiningSchema.html">PMML
 * MiningSchema</a>.
 *
 * It is a collection of MiningFields. See MiningField.
 *
 * Notice that the link with the values defined in DataDictionary is performed
 * by deriving MiningFields from DataFields.
 *
 * Through the method validate, the presence of all fields needed by the model
 * is checked.
 */
class MiningSchema {
 public:
  std::unordered_map<std::string, MiningField> raw_miningfields;
  std::vector<MiningField> miningfields;
  MiningField target;
  size_t target_index = std::numeric_limits<size_t>::max();

  MiningSchema() = default;

  MiningSchema(const XmlNode &node, const DataDictionary &data_dictionary)
      : raw_miningfields(MiningField::to_miningfields(node.get_childs("MiningField"), data_dictionary)),
        miningfields(to_values<std::string, MiningField>(raw_miningfields)),
        target(get_target(miningfields)),
        target_index(target.index) {}

  inline static MiningField get_target(const std::vector<MiningField> &miningfields) {
    for (const auto &miningfield : miningfields) {
      if (miningfield.field_usage_type == FieldUsageType::FieldUsageTypeValue::TARGET) return miningfield;
    }

    return MiningField();
  }

  inline const MiningField &operator[](const std::string &feature_name) const {
    return raw_miningfields.at(feature_name);
  }

  inline const MiningField &operator[](const size_t &feature_index) const { return miningfields[feature_index]; }

  inline bool contains(const std::string &feature_name) const {
    return raw_miningfields.find(feature_name) != raw_miningfields.cend();
  }

  inline MiningField at(const std::string &feature_name) const { return raw_miningfields.at(feature_name); }

  const void prepare(Sample &sample, const std::unordered_map<std::string, std::string> &input) const {
#ifdef DEBUG
    std::cout << "BEFORE MINING SCHEMA PREPARATION: " << sample << std::endl;
#endif
    for (const auto &miningfield : miningfields) {
      if (miningfield.index != target_index) {
        try {
          sample.change_value(miningfield.index, miningfield.createValue(input.at(miningfield.name)));
          if (miningfield.hasInvalidTreatment || miningfield.hasOutlierTreatment) {
            Value tmp_value = sample.operator[](miningfield.index).value;
            if (miningfield.hasInvalidTreatment)
              if (miningfield.is_invalid(tmp_value))
                sample.change_value(miningfield.index, miningfield.handle_invalid(tmp_value));

            if (miningfield.hasOutlierTreatment)
              if (miningfield.is_outlier(tmp_value))
                sample.change_value(miningfield.index, miningfield.handle_outlier(tmp_value));
          }

        } catch (const std::out_of_range &exception) {  // field is missing
          sample.change_value(miningfield.index, miningfield.handle_missing());
        } catch (const cpmml::Exception &exception) {  // field cannot be converted to double
                                                       // because is missing
          sample.change_value(miningfield.index, miningfield.handle_missing());
        }
      }
    }
#ifdef DEBUG
    std::cout << "AFTER MINING SCHEMA PREPARATION: " << sample << std::endl;
#endif
  }

  bool validate(const Sample &sample) const {
    for (const auto &mining_field : miningfields) {
      if (mining_field.index == target_index) continue;
      if (!mining_field.validate(sample)) return false;
    }

    return true;
  }
};

#endif
