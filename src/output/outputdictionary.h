
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_OUTPUT_H
#define CPMML_OUTPUT_H

#include "core/derivedfield.h"
#include "core/xmlnode.h"
#include "outputfield.h"

/**
 * @class OutputDictionary
 * Class representing <a href="http://dmg.org/pmml/v4-4/Output.html">PMML
 * Output</a>.
 *
 * It is a collection of OutputFields. See OutputField.
 *
 * Also in this case a DAG is built to keep track of dependencies between
 * OutputFields. However this DAG is unrelated to the one built by DagBuilder
 * since DerivedFields and OuputFields have different scope: the former deals
 * with preprocessing of fields while the second deals with postprocessing of
 * fields.
 */
class OutputDictionary {
 public:
  bool empty;
  std::unordered_map<std::string, OutputField> raw_outputfields;
  std::vector<OutputField> dag;

  OutputDictionary() : empty(true) {}

  OutputDictionary(const XmlNode &node, const std::shared_ptr<Indexer> &indexer, const std::string &model_target)
      : empty(false),
        raw_outputfields(OutputField::to_outputfields(node.get_childs("OutputField"), indexer, model_target)),
        dag(build_dag(raw_outputfields)) {}

  inline bool contains(const std::string &field_name) const {
    return raw_outputfields.find(field_name) != raw_outputfields.cend();
  }

  inline const OutputField &operator[](const std::string &feature_name) const {
    return raw_outputfields.at(feature_name);
  }

  static std::vector<OutputField> build_dag(const std::unordered_map<std::string, OutputField> &raw_outputfields) {
    std::vector<OutputField> output_fields = ::to_values<std::string, OutputField>(raw_outputfields);
    std::vector<OutputField> dag;

    for (const auto &output_field : output_fields) build_dagR(output_field, dag, raw_outputfields);

    return dag;
  }

  static void build_dagR(const OutputField &output_field, std::vector<OutputField> &dag,
                         const std::unordered_map<std::string, OutputField> &raw_outputfields) {
    for (const auto &input : output_field.expression->inputs) {
      if (raw_outputfields.find(input) != raw_outputfields.cend()) {
        OutputField tmp_input = raw_outputfields.at(input);
        if (tmp_input.derived) build_dagR(output_field, dag, raw_outputfields);

        dag.push_back(tmp_input);
      }
    }

    dag.push_back(output_field);

    return;
  }

  inline void prepare(Sample &sample) const {
    for (const auto &outputfield : dag) outputfield.prepare(sample);
  }

  inline void add_output(Sample &sample, InternalScore &score) const {
    for (const auto &outputfield : dag) outputfield.add_output(sample, score);
  }
};

#endif
