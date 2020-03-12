
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_MODELBUILDER_H
#define CPMML_MODELBUILDER_H

#include <fstream>
#include <sstream>
#include <string>

#include "datadictionary.h"
#include "ensemblemodel/ensembleevaluator.h"
#include "header.h"
#include "internal_evaluator.h"
#include "regressionmodel/regressionevaluator.h"
#include "treemodel/treeevaluator.h"
#include "treemodel/treemodel.h"
#include "xmlnode.h"

/**
 * @class ModelBuilder
 *
 * Factory class to create InternalEvaluator objects.
 */
class ModelBuilder {
 public:
  inline static std::unique_ptr<InternalEvaluator> build(const std::string &filename, const bool zipped) {
    std::vector<char> file_data = read_file(filename, zipped);
    rapidxml::xml_document<> document;
    document.parse<0>(file_data.data());
    XmlNode xmlNode(document.first_node("PMML"));
    std::unique_ptr<InternalEvaluator> evaluator;
    if (xmlNode.exists_child("MiningModel"))
      evaluator = make_unique<EnsembleEvaluator>(xmlNode);
    else if (xmlNode.exists_child("RegressionModel"))
      evaluator = make_unique<RegressionEvaluator>(xmlNode);
    else if (xmlNode.exists_child("TreeModel"))
      evaluator = make_unique<TreeEvaluator>(xmlNode);
    else
      throw cpmml::ParsingException("unsupported model type");

    document.clear();

    return evaluator;
  }
};

#endif
