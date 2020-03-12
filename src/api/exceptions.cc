
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#include "cPMML.h"

namespace cpmml {
Exception::Exception(const std::string &message) : message(message) {}

const char *Exception::what() const noexcept { return (message).c_str(); }

MissingValueException::MissingValueException(const std::string &message) : Exception(message) {}

InvalidValueException::InvalidValueException(const std::string &message) : Exception(message) {}

MathException::MathException(const std::string &message) : Exception(message) {}

ParsingException::ParsingException(const std::string &message) : Exception(message) {}
}  // namespace cpmml
