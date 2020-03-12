
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_APPLY_H
#define CPMML_APPLY_H

#include "constant.h"
#include "core/builtinfunctions.h"
#include "discretize.h"
#include "expression.h"
#include "expressionbuilder.h"
#include "expressiontype.h"
#include "fieldref.h"
#include "mapvalues.h"
#include "normcontinuous.h"
#include "normdiscrete.h"

/**
 * @class Apply
 *
 * Class representing <a
 * href="http://dmg.org/pmml/v4-4/Functions.html#xsdElement_Apply">PMML
 * Apply</a>.
 *
 * It allows to perform user-defined transformations made of other PMML
 * Expressions or PMML Built-in functions.
 */
class Apply : public Expression {
 public:
  BuiltInFunction function;
  bool exist_missingreplacement = false;
  Value mapmissing_to;
  bool exist_defaultvalue = false;
  Value defaultValue;
  InvalidValueTreatmentMethod invalidValueTreatmentMethod;
  std::vector<std::shared_ptr<Expression>> expressions;

  Apply() = default;

  Apply(const XmlNode &node, const size_t &output_index, const DataType &output_type,
        const std::shared_ptr<Indexer> &indexer)
      : Expression(output_index, output_type, indexer),
        function(BuiltInFunction(node.get_attribute("function"))),
        exist_missingreplacement(node.exists_attribute("mapMissingTo")),
        mapmissing_to(exist_missingreplacement ? Value(node.get_attribute("mapMissingTo"), output_type) : Value()),
        exist_defaultvalue(node.exists_attribute("defaultValue")),
        defaultValue(exist_defaultvalue ? Value(node.get_attribute("defaultValue"), output_type) : Value()),
        invalidValueTreatmentMethod(node.get_attribute("invalidValueTreatment")) {
    std::shared_ptr<Expression> expression;
    for (const auto &n : node.get_childs()) {
      ExpressionType expression_type(n.name());
      switch (expression_type.value) {
        case ExpressionType::ExpressionTypeValue::CONSTANT:
          expression = std::make_shared<Constant>(n, output_index, output_type, indexer);
          break;  // to be refined in case of Parameter fields
        case ExpressionType::ExpressionTypeValue::FIELD_REF:
          expression = std::make_shared<FieldRef>(n, output_index, output_type, indexer);
          break;
        case ExpressionType::ExpressionTypeValue::NORM_CONTINUOUS:
          expression = std::make_shared<NormContinuous>(n, output_index, output_type, indexer);
          break;
        case ExpressionType::ExpressionTypeValue::NORM_DISCRETE:
          expression = std::make_shared<NormDiscrete>(n, output_index, output_type, indexer);
          break;
        case ExpressionType::ExpressionTypeValue::DISCREZITE:
          expression = std::make_shared<Discretize>(n, output_index, output_type, indexer);
          break;
        case ExpressionType::ExpressionTypeValue::APPLY:
          expression = std::make_shared<Apply>(n, output_index, output_type, indexer);
          break;
        case ExpressionType::ExpressionTypeValue::MAP_VALUES:
          expression = std::make_shared<MapValues>(n, output_index, output_type, indexer);
          break;
        default:
          throw cpmml::ParsingException("Expression " + expression_type.to_string() + " type not supported");
      }

      for (const auto &input : expression->inputs) inputs.insert(input);

      expressions.push_back(expression);
    }
  }

  inline Value eval(Sample &sample) const override {
    std::vector<Value> input;
    Value tmp;
    bool missing_input = false;
    for (const auto &expression : expressions) {
      tmp = expression->eval(sample);
      input.push_back(tmp);
      if (tmp.missing) missing_input = true;
    }

    if (missing_input) {
      if (exist_missingreplacement)
        return mapmissing_to;
      else if (exist_defaultvalue)
        return defaultValue;
      else
        return Value();
    }

    Value result;

    try {  // there is no rule in PMML for validation at this level, here
           // invalid is like "division by zero"
      // thus it is captured with an exception
      result = function(input);
    } catch (const std::exception &e) {
      std::cout << e.what() << std::endl;
      switch (invalidValueTreatmentMethod.value) {
        case InvalidValueTreatmentMethod::InvalidValueTreatmentMethodValue::RETURN_INVALID:
          throw cpmml::InvalidValueException("evaluating apply function");
        case InvalidValueTreatmentMethod::InvalidValueTreatmentMethodValue::AS_MISSING:
          if (exist_missingreplacement)
            return mapmissing_to;
          else if (exist_defaultvalue)
            return defaultValue;
          else
            return Value();
        case InvalidValueTreatmentMethod::InvalidValueTreatmentMethodValue::AS_IS:
          break;
      }
    }

    return result;
  }
};

#endif
