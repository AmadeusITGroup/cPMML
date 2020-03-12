
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_MODEL_H
#define CPMML_MODEL_H

#include <string>

#include "dagbuilder.h"
#include "miningfunction.h"
#include "miningschema.h"
#include "output/outputdictionary.h"
#include "target.h"
#include "transformationdictionary.h"

/**
 * @class InternalModel
 *
 * Abstract class representing a <a
 * href="http://dmg.org/pmml/v4-4/GeneralStructure.html#xsdGroup_MODEL-ELEMENT">PMML
 * MODEL-ELEMENT</a>.
 *
 * Implementations of this class are the central element of cPMML and they are
 * the ones actually producing the scoring.
 *
 * It contains:
 * - MiningSchema
 * - LocalTransformations
 * - Target
 * - Output
 *
 * Through the method validate, the presence of all fields needed by the model
 * is checked. This includes DerivedFields obtained through
 * TransformationDictionary and also LocalTransformations.
 */
class InternalModel {
 public:
  std::shared_ptr<Indexer> indexer;
  MiningFunction mining_function;
  MiningSchema mining_schema;
  MiningField target_field;
  TransformationDictionary transformation_dictionary;
  bool has_local_transformations = false;
  Target target;
  OutputDictionary output;
  Sample base_sample;
  std::vector<std::string> derivedfields_dag;

  InternalModel() = default;

  InternalModel(const XmlNode &node, const DataDictionary &data_dictionary, const std::shared_ptr<Indexer> &indexer)
      : indexer(indexer),
        mining_function(node.get_attribute("functionName")),
        mining_schema(MiningSchema(node.get_child("MiningSchema"), data_dictionary)),
        target_field(get_target(mining_function, mining_schema, indexer)),
        has_local_transformations(false),
        target(get_target(node, mining_schema, transformation_dictionary, mining_function)),
        output(get_output(node, indexer, target_field.name)) {
    check_scorable(node);
  }

  InternalModel(const XmlNode &node, const DataDictionary &data_dictionary,
                const TransformationDictionary &transformation_dictionary, const std::shared_ptr<Indexer> &indexer)
      : indexer(indexer),
        mining_function(node.get_attribute("functionName")),
        mining_schema(MiningSchema(node.get_child("MiningSchema"), data_dictionary)),
        target_field(get_target(mining_function, mining_schema, indexer)),
        transformation_dictionary(transformation_dictionary),
        has_local_transformations(add_local_transformations(node, this->transformation_dictionary, indexer)),
        target(get_target(node, mining_schema, this->transformation_dictionary, mining_function)),
        output(get_output(node, indexer, target_field.name)),
        base_sample(create_basesample(indexer)),
        derivedfields_dag(DagBuilder::build(mining_schema, this->transformation_dictionary)) {
    check_scorable(node);
  }

  inline bool validate(const std::unordered_map<std::string, std::string> &sample) const {
    Sample internal_sample = base_sample;
    mining_schema.prepare(internal_sample, sample);
    if (!transformation_dictionary.empty)
      for (const auto &derivedfield_name : derivedfields_dag)
        transformation_dictionary[derivedfield_name].prepare(internal_sample);

    return mining_schema.validate(internal_sample);
  }

  inline void augment_first(Sample &sample) const {
    if (!transformation_dictionary.empty)
      for (const auto &derivedfield_name : derivedfields_dag)
        transformation_dictionary[derivedfield_name].prepare(sample);

    sample.change_value(indexer->get_index(target_field.name),
                        Value(target(predict_raw(sample)), target_field.datatype));

    output.prepare(sample);
  };

  inline void augment(Sample &sample) const {
    sample.change_value(indexer->get_index(target_field.name),
                        Value(target(predict_raw(sample)), target_field.datatype));

    output.prepare(sample);
  };

  inline std::unique_ptr<InternalScore> augment_last(Sample &sample) const {
    std::unique_ptr<InternalScore> score = score_raw(sample);
    target(*score);
    output.add_output(sample, *score);

    return score;
  };

  inline std::unique_ptr<InternalScore> score(const std::unordered_map<std::string, std::string> &sample) const {
    Sample internal_sample = base_sample;
    mining_schema.prepare(internal_sample, sample);
    if (!transformation_dictionary.empty)
      for (const auto &derivedfield_name : derivedfields_dag)
        transformation_dictionary[derivedfield_name].prepare(internal_sample);

    if (!mining_schema.validate(internal_sample))
      throw cpmml::InvalidValueException("Sample: " + to_string(sample) + "didn't pass input validation");

    std::unique_ptr<InternalScore> score = score_raw(internal_sample);
    target(*score);
    output.add_output(internal_sample, *score);

    return score;
  };

  virtual std::unique_ptr<InternalScore> score_raw(const Sample &sample) const = 0;

  inline std::string predict(const std::unordered_map<std::string, std::string> &sample) const {
    Sample internal_sample = base_sample;
    mining_schema.prepare(internal_sample, sample);
    if (!transformation_dictionary.empty)
      for (const auto &derivedfield_name : derivedfields_dag)
        transformation_dictionary[derivedfield_name].prepare(internal_sample);

    if (!mining_schema.validate(internal_sample))
      throw cpmml::InvalidValueException("Sample: " + to_string(sample) + "didn't pass input validation");

    return target(predict_raw(internal_sample));
  };

  static inline Target get_target(const XmlNode &node, const MiningSchema &mining_schema,
                                  const TransformationDictionary &transformation_dictionary,
                                  const MiningFunction &mining_function) {
    if (node.exists_child("Targets"))
      if (node.get_child("Targets").exists_child("Target"))
        return Target(node.get_child("Targets").get_child("Target"), mining_schema, transformation_dictionary,
                      mining_function);

    return Target();
  }

  static inline OutputDictionary get_output(const XmlNode &node, const std::shared_ptr<Indexer> &indexer,
                                            const std::string &target_field_name) {
    return node.exists_child("Output") ? OutputDictionary(node.get_child("Output"), indexer, target_field_name)
                                       : OutputDictionary();
  }

  static inline void check_scorable(const XmlNode &node) {
    if (node.exists_attribute("isScorable") && node.get_bool_attribute("isScorable") == false)
      throw cpmml::ParsingException("The model is defined as non-scorable");
  }

  static inline bool add_local_transformations(const XmlNode &node, TransformationDictionary &transformation_dictionary,
                                               const std::shared_ptr<Indexer> &indexer) {
    if (node.exists_child("LocalTransformations")) {
      for (auto const &n : node.get_child("LocalTransformations").get_childs("DerivedField"))
        transformation_dictionary.add_derived_field(DerivedField(n, indexer));
      return true;
    }

    return false;
  }

  static Sample create_basesample(const std::shared_ptr<Indexer> &indexer) {
    Sample sample(indexer->size());
    for (const auto &index : *indexer) sample[index.second] = Feature(index.first, Value());

    return sample;
  }

  static MiningField get_target(const MiningFunction &mining_function, const MiningSchema &mining_schema,
                                const std::shared_ptr<Indexer> &indexer) {
    if (mining_schema.target.empty) {
      DataType datatype = mining_function.value == MiningFunction::MiningFunctionType::CLASSIFICATION
                              ? DataType::DataTypeValue::STRING
                              : DataType::DataTypeValue::DOUBLE;
      MiningField target_placeholder(indexer->random_name(), datatype);
      indexer->get_or_set(target_placeholder.name, target_placeholder.datatype);

      return target_placeholder;
    }

    return mining_schema.target;
  }

  virtual std::string predict_raw(const Sample &sample) const = 0;

  InternalModel(const InternalModel &) = default;

  InternalModel(InternalModel &&) = default;

  InternalModel &operator=(const InternalModel &) = default;

  InternalModel &operator=(InternalModel &&) = default;

  virtual ~InternalModel() = default;
};

#endif
