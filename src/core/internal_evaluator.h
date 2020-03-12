
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_EVALUATOR_H
#define CPMML_EVALUATOR_H

#include <sstream>
#include <string>

#include "datadictionary.h"
#include "header.h"
#include "internal_score.h"
#include "options.h"
#include "transformationdictionary.h"

#include "indexer.h"

/**
 * @class InternalEvaluator
 *
 * Class encapsulating the high level elements of the <a
 * href="http://dmg.org/pmml/v4-4/GeneralStructure.html">PMML document</a>.:
 *
 * It contains:
 *  - Header
 *  - DataDictionary
 *  - TransformationDictionary
 *  - MODEL-ELEMENT
 */
class InternalEvaluator {
 public:
  std::shared_ptr<Indexer> indexer;
  std::string name;
  std::string version;
  Header header;
  DataDictionary data_dictionary;
  bool hasPreprocessing = false;
  TransformationDictionary transformation_dictionary;

  InternalEvaluator() = default;

  InternalEvaluator(const XmlNode &node)
      : indexer(new Indexer()),
        name(node.name()),
        version(node.get_attribute("version")),
        header(node.get_child("Header")),
        data_dictionary(node.get_child("DataDictionary"), indexer),
        hasPreprocessing(node.exists_child("TransformationDictionary")),
        transformation_dictionary(hasPreprocessing
                                      ? TransformationDictionary(node.get_child("TransformationDictionary"), indexer)
                                      : TransformationDictionary()){};

  virtual inline bool validate(const std::unordered_map<std::string, std::string> &sample) { return false; }

  virtual std::unique_ptr<InternalScore> score(const std::unordered_map<std::string, std::string> &sample) const = 0;

  // Simple score, due to the type of value returned is 2/300 ns faster
  virtual std::string predict(const std::unordered_map<std::string, std::string> &sample) const = 0;

  virtual inline std::string get_target_name() const { return ""; }

  InternalEvaluator(const InternalEvaluator &) = default;

  InternalEvaluator(InternalEvaluator &&) = default;

  InternalEvaluator &operator=(const InternalEvaluator &) = default;

  InternalEvaluator &operator=(InternalEvaluator &&) = default;

  virtual ~InternalEvaluator() = default;
};

#endif
