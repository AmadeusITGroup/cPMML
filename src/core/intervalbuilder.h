
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_INTERVALBUILDER_H
#define CPMML_INTERVALBUILDER_H

#include "predicate.h"
#include "xmlnode.h"

/**
 * @class IntervalBuilder
 *
 * Class used to build the constraints for continous features of the
 * DataDictionary. See also Closure.
 */
class IntervalBuilder {
 public:
  static Predicate build(const XmlNode &node, const size_t &index,
                         const DataType &dataType = DataType::DataTypeValue::DOUBLE) {
    std::vector<Predicate> tmp_constraints;

    Closure closure(node.get_attribute("closure"));
    Value left_margin = node.exists_attribute("leftMargin") ? Value(node.get_attribute("leftMargin"), dataType)
                                                            : Value(std::numeric_limits<double>::max(), dataType);
    Value right_margin = node.exists_attribute("rightMargin") ? Value(node.get_attribute("rightMargin"), dataType)
                                                              : Value(std::numeric_limits<double>::max(), dataType);
    switch (closure.value) {
      case Closure::ClosureValue::CLOSED_CLOSED:
        tmp_constraints.push_back(Predicate(index, "greaterOrEqual", left_margin));
        tmp_constraints.push_back(Predicate(index, "lessOrEqual", right_margin));
        break;
      case Closure::ClosureValue::OPEN_OPEN:
        tmp_constraints.push_back(Predicate(index, "greaterThan", left_margin));
        tmp_constraints.push_back(Predicate(index, "lessThan", right_margin));
        break;
      case Closure::ClosureValue::CLOSED_OPEN:
        tmp_constraints.push_back(Predicate(index, "greaterOrEqual", left_margin));
        tmp_constraints.push_back(Predicate(index, "lessThan", right_margin));
        break;
      case Closure::ClosureValue::OPEN_CLOSED:
        tmp_constraints.push_back(Predicate(index, "greaterThan", left_margin));
        tmp_constraints.push_back(Predicate(index, "lessOrEqual", right_margin));
        break;
    }

    return Predicate(tmp_constraints, "AND");
  }
};

#endif
