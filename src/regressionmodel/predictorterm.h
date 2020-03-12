
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_PREDICTORTERM_H
#define CPMML_PREDICTORTERM_H

#include <string>

#include "core/datatype.h"
#include "core/sample.h"
#include "core/value.h"
#include "core/xmlnode.h"

/**
 * @class PredictorTerm
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Regression.html#xsdElement_PredictorTerm">PMML
 * PredictorTerm</a>.
 *
 * It contains references to other fields in the PMML, which are combined by
 * multiplication.
 */
class PredictorTerm {
  // this could be treated as a derived field visible in Statistics,
  // since Statistics is out of scope (only metadata), that visibility is not
  // implemented for the same reason SimpleFielfRef is used instead of FieldRef

 public:
  std::string name;
  DataType datatype;
  double coefficient = double_min();
  std::vector<SimpleFieldRef> fields;
  std::function<double(const Value &)> predictor_term;

  PredictorTerm() = default;

  PredictorTerm(const XmlNode &node,  // PredictorTerm node
                const std::shared_ptr<Indexer> &indexer)
      : name(node.get_attribute("name")),
        coefficient(node.get_double_attribute("coefficient")),
        fields(SimpleFieldRef::to_simplefields(node.get_childs("FieldRef"), indexer)) {}

  inline double get_term(const Sample &sample) const {
    double partial = 1;
    for (const auto &field : fields) {
      const Value &value = field.value(sample);
      if (value.missing)
        throw cpmml::MissingValueException("missing value index: " + std::to_string(field.index) +
                                           " name: " + field.field_name);
      else
        partial *= value.value;
    }

    return coefficient * partial;
  }

  static const std::vector<PredictorTerm> to_predictorterms(const std::vector<XmlNode> &nodes,
                                                            const std::shared_ptr<Indexer> &indexer) {
    std::vector<PredictorTerm> result;

    for (const auto &node : nodes) result.push_back(PredictorTerm(node, indexer));

    return result;
  }
};

#endif
